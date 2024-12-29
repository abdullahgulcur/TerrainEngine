#version 460 core

layout (location = 0) in uvec2 posVert;
layout (location = 1) in uvec2 posInstance;
layout (location = 2) in uint levelInstance;

uniform int blockSize;
uniform ivec2 terrainSize;
uniform mat4 projectionView;
uniform usampler2D pageTable;
uniform usampler2D physicalHeightmap;

out vec2 WorldPos2D;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1 << bits) - 1);
}

uint getScale(uvec2 pos2D, ivec2 offset){

    uvec2 posInPage = (pos2D + offset) / blockSize;
    uint color = texelFetch(pageTable, ivec2(posInPage), 0).r;
    uint scale = 1 << getValue(color, 0, 4);
    return scale;
}

uvec2 getTexelCoordinate(uvec2 pos2D){

    uvec2 posInPage = pos2D / blockSize;
    uint color = texelFetch(pageTable, ivec2(posInPage), 0).r;
    uint scale = 1 << getValue(color, 0, 4);
    uint pageSizeMultiplier = blockSize * scale;
    uvec2 pageStartWorld = uvec2(getValue(color, 22, 10), getValue(color, 12, 10)) * pageSizeMultiplier;
    uvec2 offsetWorld = pos2D - pageStartWorld;
    uvec2 offsetInPhysicalPage = offsetWorld / scale;
    uvec2 pagePos = uvec2(getValue(color, 8, 4), getValue(color, 4, 4));
    uvec2 currentPagePositionInTexture = pagePos * blockSize;
    return currentPagePositionInTexture + offsetInPhysicalPage;
}

float getHeightFromWorldPos2D(uvec2 worldPos2D){
    uvec2 texelCoord = getTexelCoordinate(worldPos2D);
    uint color = texelFetch(physicalHeightmap, ivec2(texelCoord), 0).r;
    float height = color / 1500.f;
    return height;
}

float getNeightborHeightFromWorldPos2D(uvec2 worldPos2D, uvec2 direction, uint scale){
    
    uvec2 offset = scale * direction;
    return getHeightFromWorldPos2D(worldPos2D + offset);
}

void main(void)
{
    uint scale = 1 << levelInstance;

    uvec2 worldPos2D = (posVert + posInstance * blockSize) * scale;
    float height = getHeightFromWorldPos2D(worldPos2D);
    vec3 position = vec3(worldPos2D.x, height, worldPos2D.y);

    //---------

    uint s0 = getScale(worldPos2D, ivec2(0, -1));
    float h0 = getNeightborHeightFromWorldPos2D(worldPos2D, ivec2(0, -1), s0);

    uint s1 = getScale(worldPos2D, ivec2(-1, 0));
    float h1 = getNeightborHeightFromWorldPos2D(worldPos2D, ivec2(-1, 0), s1);

    uint s2 = getScale(worldPos2D, ivec2(1, 0));
    float h2 = getNeightborHeightFromWorldPos2D(worldPos2D, ivec2(1, 0), s2);

    uint s3 = getScale(worldPos2D, ivec2(0, 1));
    float h3 = getNeightborHeightFromWorldPos2D(worldPos2D, ivec2(0, 1), s3);

    //---------

    vec3 normal;
	normal.z = ((h0 - height) / s0) + ((height - h3) / s3);
	normal.x = ((h1 - height) / s1) + ((height - h2) / s2);
	normal.y = 2;
	normal = normalize(normal);

    //--------- 

    vec3 tangent;
    tangent.x = 2;
    tangent.y = ((h2 - height) / s2) + ((height - h1) / s1);
    tangent.z = 0;
	tangent = normalize(tangent);

    //---------

    vec3 bitangent = -normalize(cross(tangent, normal));
    TBN = mat3(tangent, bitangent, normal);

    //---------

    WorldPos2D = vec2(worldPos2D);
    WorldPos = position;
    Normal = normal;

    //---------

    gl_Position = projectionView * vec4(position, 1.0);
}