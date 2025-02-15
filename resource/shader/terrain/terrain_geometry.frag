#version 460 core

in vec2 WorldPos2D;
in vec3 WorldPos;
out vec4 FragColor;

uniform usampler2D pageTable;
uniform sampler2DArray physicalPages;
//uniform uvec2 pageCounts;
uniform vec3 cameraPosition;
uniform uint totalMipmapLevel;
//uniform uint currentMipmapIndexForShader;
uniform uvec2 mipCounts;
uniform int mipmapLevelList[16];

uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1u << bits) - 1);
}

int getLODLevel1(vec3 worldPosition) {

    float margin = 0.2;
    float baseDistance = 2 * (1 - margin);
    float dist = distance(cameraPosition, worldPosition);
    
    int lodLevel = int(floor(log2(dist / (baseDistance * 0.5))));
    int maxLodLevel = int(totalMipmapLevel - 1);
    return clamp(lodLevel, 0, maxLodLevel);
}

void getUV(uint mipmapLevel, inout uint pageIndex, inout vec2 uv){

    uvec2 mipmapIndex = uvec2(mipmapLevel % mipCounts.x, mipmapLevel / mipCounts.y);

    vec2 pageIndexF = (WorldPos2D + 0.5 * (1 << mipmapLevel)) / (1 << mipmapLevel); // AAA  + 0.5 * (1 << mipmapLevel)
    //pageIndexF = pageIndexF * 1.02 - 0.01;
    uv = fract(pageIndexF);// + 0.5 / 1024;// - 0.5 / textureSize(physicalPages, int(mipmapLevel)).x;// + vec2(0.5 / textureSize(physicalPages, int(mipmapLevel)).x);;// + vec2(0.5 / textureSize(physicalPages, 0).x);

    float border = 0.045;
    uv = uv * (1 - border * 2) + border;

    //uv = (uv + border) * (1 / (1 + border));

    //uv = uv * 1.02 - 0.01;
    //uv = mix(vec2(0.02), vec2(0.98), uv);
    //uv = clamp(uv, vec2(0.01), vec2(0.99));
    uvec2 texelIndex = uvec2(pageIndexF) % uvec2(5);
    texelIndex += mipmapIndex * 5;

    uint color = texelFetch(pageTable, ivec2(texelIndex), 0).r;
    pageIndex = color;
}

float computeMipLevel(vec2 texCoords) {
    vec2 dx = dFdx(texCoords);
    vec2 dy = dFdy(texCoords);
    float deltaMax = max(dot(dx, dx), dot(dy, dy));
    return 0.5 * log2(deltaMax);  // This is the approximate mip level
}

void main(){

    float margin = 0.2;
    float baseDistance = 2 * (1 - margin);

    float uniformDistance = 0.25;

    int maxLodLevel = int(totalMipmapLevel - 1);

    int mipLevel = getLODLevel1(WorldPos);

    int mipLevelTemp = max(mipLevel, 1);
    float lodDistanceFiner = (1 << (mipLevelTemp - 1)) * baseDistance;
    int zeroFlag = 1 - int(step(mipLevel, 0));
    lodDistanceFiner *= zeroFlag;
    float lodDistanceCoarse = (1 << mipLevel) * baseDistance;
    float dist = distance(cameraPosition, WorldPos);
    float ratio = (dist - lodDistanceFiner) / (lodDistanceCoarse - lodDistanceFiner);
    ratio = max(0, ratio - uniformDistance) * (1 / (1 - uniformDistance));
    ratio = clamp(ratio, 0, 1);

    //float border = step(0.9, ratio) * step(0.1, ratio);
    //ratio = smoothstep(0.0, 1.0, ratio);

    //uint mipLevelIndex = max(currentMipmapIndexForShader, uint(mipLevel));
    uint mipLevelFiner = mipmapLevelList[mipLevel];
    uint mipLevelCoarse = mipmapLevelList[min(mipLevel + 1, maxLodLevel)];
    uint mipLevelCoarse1 = mipmapLevelList[min(mipLevel + 2, maxLodLevel)];

    vec2 uvFiner;
    uint pageIndexFiner;
    getUV(mipLevelFiner, pageIndexFiner, uvFiner);

    vec2 uvCoarse;
    uint pageIndexCoarse;
    getUV(mipLevelCoarse, pageIndexCoarse, uvCoarse);

//    vec2 uvCoarse1;
//    uint pageIndexCoarse1;
//    getUV(mipLevelCoarse1, pageIndexCoarse1, uvCoarse1);

//    float mip0 = computeMipLevel(uvFiner);
//    float mip1 = computeMipLevel(uvCoarse);

//    vec3 albedoFinerManualMipmap = textureLod(physicalPages, vec3(uvFiner, pageIndexFiner), mip0).rgb;
//    vec3 albedoCoarseManualMipmap = textureLod(physicalPages, vec3(uvCoarse, pageIndexCoarse), mip1).rgb;

//    uvFiner = uvFiner * 0.9 + 0.05;
//    uvCoarse = uvCoarse * 0.9 + 0.05;

    vec3 albedoFinerAutoMipmap = texture(physicalPages, vec3(uvFiner, pageIndexFiner)).rgb;
    vec3 albedoCoarseAutoMipmap = texture(physicalPages, vec3(uvCoarse, pageIndexCoarse)).rgb;
    //vec3 albedoCoarseAutoMipmap1 = texture(physicalPages, vec3(uvCoarse1, pageIndexCoarse1)).rgb;

//    vec3 albedoFiner = mix(albedoFinerAutoMipmap, albedoFinerManualMipmap, border);
//    vec3 albedoCoarse = mix(albedoCoarseAutoMipmap, albedoCoarseManualMipmap, border);

    vec3 color = mix(albedoFinerAutoMipmap, albedoCoarseAutoMipmap, ratio);
    //color = (color + albedoCoarseAutoMipmap1) * 0.5;
    FragColor = vec4(color, 1);// * mipLevelFiner * 0.1
    //FragColor = vec4(vec3(vec3(mipLevelFiner * 0.1)), 1);

    //FragColor = vec4(vec3(albedoFinerAutoMipmap), 1);
}
