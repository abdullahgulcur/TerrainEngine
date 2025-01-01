#version 460 core

layout (location = 0) in uvec2 posVert;
layout (location = 1) in uvec2 posInstance;
layout (location = 2) in uint levelInstance;

uniform int blockSize;
uniform mat4 projectionView;
uniform usampler2D pageTable;
uniform usampler2D physicalHeightmap;

out vec2 WorldPos2D;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1u << bits) - 1);
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

uint getValueFromWorldPos2D(uvec2 worldPos2D){
    uvec2 texelCoord = getTexelCoordinate(worldPos2D);
    return texelFetch(physicalHeightmap, ivec2(texelCoord), 0).r;
}

vec3 getNormal(float dxF, float dyF){

    vec3 normal;
    normal.x = dxF;
    normal.y = 2;
    normal.z = dyF;
    return normalize(normal);
}

vec3 getTangent(float dxF){

    vec3 tangent;
    tangent.x = 2;
    tangent.y = -dxF;
    tangent.z = 0;
    return normalize(tangent);
}

void main(void)
{
    float heightScale = 0.03; //uniform ?

    uint scale = 1 << levelInstance;

    uvec2 worldPos2D = (posVert + posInstance * blockSize) * scale;
    uint color = getValueFromWorldPos2D(worldPos2D);
    uint height = getValue(color, 0, 16);
    uint dx = getValue(color, 24, 7);
    uint dy = getValue(color, 16, 7);
    int dxSign = int(getValue(color, 31, 1));
    int dySign = int(getValue(color, 23, 1));
    dxSign = (dxSign * (-2)) + 1;
    dySign = (dySign * (-2)) + 1;
    float dxF = int(dx) * dxSign * heightScale;
    float dyF = int(dy) * dySign * heightScale;

    float heightF = height * heightScale;

    vec3 position = vec3(worldPos2D.x, heightF, worldPos2D.y);

    //---------

    vec3 normal = getNormal(dxF, dyF);
    vec3 tangent = getTangent(dxF);
    vec3 bitangent = -normalize(cross(tangent, normal));
    mat3 tbn = mat3(tangent, bitangent, normal);

    //---------

    WorldPos2D = vec2(worldPos2D);
    WorldPos = position;
    Normal = normal;
    TBN = tbn;

    //---------

    gl_Position = projectionView * vec4(position, 1.0);
}