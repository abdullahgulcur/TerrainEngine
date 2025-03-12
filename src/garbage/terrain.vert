#version 460 core

layout (location = 0) in uvec2 posVert;
layout (location = 1) in uvec2 posInstance;
layout (location = 2) in uint levelInstance;

uniform uint pageTableSize;
uniform uint totalMipmapLevel;
uniform int blockSize;
uniform mat4 projectionView;
uniform usampler2D pageTable;
uniform usampler2D physicalHeightmap;
uniform vec3 cameraPosition;
uniform sampler2D physicalPagesDisplacement;

out vec2 WorldPos2D;
out vec3 WorldPos;
out vec3 Normal;
out vec3 Tangent;

//--------------- COMMON ---------------
uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1u << bits) - 1);
}

float bilinearInterpolation(float val0, float val1, float val2, float val3, vec2 texelGrad){
    float valX0 = mix(val0, val1, texelGrad.x);
    float valX1 = mix(val2, val3, texelGrad.x);
    float valFinal = mix(valX0, valX1, texelGrad.y);
    return valFinal;
}

float getHeight(uint color){

    float heightScale = 0.01; //uniform ?
    return color * heightScale;
}

void getValues(uint color, inout float dxF, inout float dyF, inout float heightF){

    float heightScale = 0.01; //uniform ?

    uint height = getValue(color, 0, 16);
    uint dx = getValue(color, 24, 7);
    uint dy = getValue(color, 16, 7);
    int dxSign = int(getValue(color, 31, 1));
    int dySign = int(getValue(color, 23, 1));
    dxSign = (dxSign * (-2)) + 1;
    dySign = (dySign * (-2)) + 1;

    dxF = int(dx) * dxSign * heightScale;
    dyF = int(dy) * dySign * heightScale;
    heightF = height * heightScale;
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

//------------------------------WORLD SPACE TO TERRAIN SPACE !!!!!!!!!!!!!!!

//uvec2 getTexelCoordinate(uvec2 pos2D){
//
//    uvec2 posInPage = pos2D / blockSize;
//    uint color = texelFetch(pageTable, ivec2(posInPage), 0).r;
//
//    uint finerColor = getValue(color, 0, 12);
//
//    uint scale = 1 << getValue(finerColor, 0, 4);
//    uint pageSizeMultiplier = blockSize * scale;
//    uvec2 pageStartWorld = (pos2D / pageSizeMultiplier) * pageSizeMultiplier;
//    uvec2 offsetWorld = pos2D - pageStartWorld;
//    uvec2 offsetInPhysicalPage = offsetWorld / scale;
//    uvec2 pagePos = uvec2(getValue(finerColor, 8, 4), getValue(finerColor, 4, 4));
//    uvec2 currentPagePositionInTexture = pagePos * blockSize;
//    return currentPagePositionInTexture + offsetInPhysicalPage;
//}

int getLODLevel(vec2 worldPosition2D) {

    float margin = 0.1;
    float baseDistance = blockSize * (2 - margin);
    float dist = distance(vec2(cameraPosition.x, cameraPosition.z), worldPosition2D);
    
    int lodLevel = int(floor(log2(dist / (baseDistance * 0.5))));
    int maxLodLevel = int(totalMipmapLevel - 1);
    return clamp(lodLevel, 0, maxLodLevel);
}

uint getLateralOffset(uint level){

    uint pageTableSizeX = pageTableSize;
	uint xOffset = 0;
	uint maxLevel = totalMipmapLevel - 1;
	for (uint i = 0; i < level; i++) {
		xOffset += pageTableSizeX;
		pageTableSizeX >>= 1;
	}
    return xOffset;
}

uvec2 getTexelCoordinate(uvec2 worldPosition2D, uint color, uint scale){
    
    uint pageSizeMultiplier = blockSize * scale;
    uvec2 pageStartWorld = (worldPosition2D / pageSizeMultiplier) * pageSizeMultiplier;
    uvec2 offsetWorld = worldPosition2D - pageStartWorld;
    uvec2 offsetInPhysicalPage = offsetWorld / scale;
    uvec2 pagePos = uvec2(getValue(color, 8, 4), getValue(color, 4, 4));
    uvec2 currentPagePositionInTexture = pagePos * blockSize;
    return currentPagePositionInTexture + offsetInPhysicalPage;
}

uvec2 getTexelCoordinate1(uvec2 worldPosition2D, uint color, uint scale, inout vec2 blend){
    
    uint pageSizeMultiplier = blockSize * scale;
    uvec2 pageStartWorld = (worldPosition2D / pageSizeMultiplier) * pageSizeMultiplier;
    uvec2 offsetWorld = worldPosition2D - pageStartWorld;

    vec2 offsetInPhysicalPageF = offsetWorld / float(scale);
    uvec2 offsetInPhysicalPage = uvec2(offsetInPhysicalPageF);
    blend = offsetInPhysicalPageF - offsetInPhysicalPage;

    uvec2 pagePos = uvec2(getValue(color, 8, 4), getValue(color, 4, 4));
    uvec2 currentPagePositionInTexture = pagePos * blockSize;
    return currentPagePositionInTexture + offsetInPhysicalPage;
}

//void getTexelCoordinates(uvec2 pos2D, inout uvec2 finerUV, inout uvec2 xUV){
//
//    uvec2 posInPage = pos2D / blockSize;
//    uint color = texelFetch(pageTable, ivec2(posInPage), 0).r;
//
//    uint finerColor = getValue(color, 0, 12);
//    uint xColor = getValue(color, 12, 12);
//
//    finerUV = getTexelCoordinate(pos2D, finerColor);
//    xUV = getTexelCoordinate(pos2D, xColor);
//}

uint getColor(uvec2 worldPos2D, uint level){

    uint xOffset = getLateralOffset(level);
    uint scale = 1 << level;

    uvec2 posInPage = worldPos2D / (blockSize * scale);
    posInPage.x += xOffset;

    uint color = texelFetch(pageTable, ivec2(posInPage), 0).r;
    uvec2 uv = getTexelCoordinate(worldPos2D, color, scale);
    return texelFetch(physicalHeightmap, ivec2(uv), 0).r;
}

float getLodDistance(uint level){
    return (1 << level) * blockSize * 1.9;
}

void getValueFromWorldPos2D(uvec2 worldPos2D, float h0, float h1){

    uint finerLevel = getLODLevel(worldPos2D);

    uint xOffset = getLateralOffset(finerLevel);
    uint scale = 1 << finerLevel;

    uvec2 posInPage = worldPos2D / (blockSize * scale);
    posInPage.x += xOffset;

    uint color = texelFetch(pageTable, ivec2(posInPage), 0).r;
    uvec2 uv = getTexelCoordinate(worldPos2D, color, scale);
    uint color0 = texelFetch(physicalHeightmap, ivec2(uv), 0).r;
    color0 = getValue(color0, 0, 16);
    h0 = getHeight(color0);

    //-----------

    uint coarseLevel = finerLevel + 1;
    xOffset = getLateralOffset(coarseLevel );
    scale = 1 << coarseLevel ;

    posInPage = worldPos2D / (blockSize * scale);
    posInPage.x += xOffset;

    color = texelFetch(pageTable, ivec2(posInPage), 0).r;
    vec2 blend;
    uv = getTexelCoordinate1(worldPos2D, color, scale, blend);

    uint color1_0 = texelFetch(physicalHeightmap, ivec2(uv), 0).r;
    uint color1_1 = texelFetch(physicalHeightmap, ivec2(uv) + ivec2(1, 0), 0).r;
    uint color1_2 = texelFetch(physicalHeightmap, ivec2(uv) + ivec2(0, 1), 0).r;
    uint color1_3 = texelFetch(physicalHeightmap, ivec2(uv) + ivec2(1, 1), 0).r;
    
    color1_0 = getValue(color1_0, 0, 16);
    color1_1 = getValue(color1_0, 0, 16);
    color1_2 = getValue(color1_0, 0, 16);
    color1_3 = getValue(color1_0, 0, 16);

    float color1_0_Height = getHeight(color1_0);
    float color1_1_Height = getHeight(color1_1);
    float color1_2_Height = getHeight(color1_2);
    float color1_3_Height = getHeight(color1_3);

    h1 = bilinearInterpolation(color1_0_Height, color1_1_Height, color1_2_Height, color1_3_Height, blend);
}

void main(void)
{
    float heightScale = 0.01; //uniform ?

    uvec2 worldPos2D = (posVert + posInstance * blockSize) * (1 << levelInstance);

    uint color0;
    uint color1;
    getValueFromWorldPos2D(worldPos2D, color0, color1);

    //---------

    float dxF;
    float dyF;
    float heightF;
//    getValues(color, dxF, dyF, heightF);

    //---------

    vec3 position = vec3(worldPos2D.x, heightF, worldPos2D.y);
    vec3 normal = getNormal(dxF, dyF);
    vec3 tangent = getTangent(dxF);
    vec3 bitangent = -normalize(cross(tangent, normal));
    mat3 tbn = mat3(tangent, bitangent, normal);

    //---------

    WorldPos2D = vec2(worldPos2D);
    WorldPos = position;
    Normal = normal;
    Tangent = tangent;

    //---------

//    gl_Position = projectionView * vec4(position, 1.0);

    uint level = getLODLevel(worldPos2D);
    gl_Position = projectionView * vec4(vec3(worldPos2D.x,0,worldPos2D.y), 1.0);
}