#version 460 core

in vec2 fragPos;
in vec2 WorldPos2D;
in vec3 WorldPos;
out vec4 FragColor;

uniform usampler2D pageTable;
uniform sampler2DArray physicalPages;
uniform vec3 cameraPosition;
uniform uint totalMipmapLevel;
uniform uvec2 mipCounts;
uniform int mipmapLevelList[16];

int getLODLevelBorder(vec3 worldPosition, inout float dist) {

    float lodDistanceFiner, lodDistanceCoarse;

    float margin = 0.2;
    float baseDistance = 2 * (1 - margin) * 1.5;
    dist = distance(cameraPosition, worldPosition);
    
    lodDistanceFiner = 0;
    for (int i = 0; i < 16; i++) {
        lodDistanceCoarse = (1 << i) * baseDistance;
        if (dist >= lodDistanceFiner && dist < lodDistanceCoarse)
            return i + 1;
        lodDistanceFiner = lodDistanceCoarse;
    }
    return -1;
}

int getLODLevelRevised(vec3 worldPosition, inout float dist, inout float lodDistanceFiner, inout float lodDistanceCoarse) {

    float margin = 0.2;
    float baseDistance = 2 * (1 - margin);
    dist = distance(cameraPosition, worldPosition);
    
    lodDistanceFiner = 0;
    for (int i = 0; i < 16; i++) {
        lodDistanceCoarse = (1 << i) * baseDistance;
        if (dist >= lodDistanceFiner && dist < lodDistanceCoarse)
            return i;
        lodDistanceFiner = lodDistanceCoarse;
    }
    return -1;
}

int getLODLevel(vec3 worldPosition) {

    float margin = 0.2;
    float baseDistance = 2 * (1 - margin);
    float dist = distance(cameraPosition, worldPosition);
    
    int lodLevel = int(floor(log2(dist / (baseDistance * 0.5))));
    int maxLodLevel = int(totalMipmapLevel - 1);
    return clamp(lodLevel, 0, maxLodLevel);
}

void getUV(uint mipmapLevel, inout uint pageIndex, inout vec2 uv, inout float pageBorder){

    uvec2 mipmapIndex = uvec2(mipmapLevel % mipCounts.x, mipmapLevel / mipCounts.y);
    vec2 pageIndexF = (WorldPos2D + 0.5 * (1 << mipmapLevel)) / (1 << mipmapLevel); // AAA  + 0.5 * (1 << mipmapLevel)

    uv = fract(pageIndexF);
    float border = 0.045;
    uv = uv * (1 - border * 2) + border;

    float temp = 0.1;
    //vec2 bbb = max(step(uv, vec2(0.1)) * smoothstep(0.0, 0.8, (0.1 - uv) / 0.1), step(vec2(0.9), uv) * smoothstep(0.0, 0.8, (uv - 0.9) / 0.1));
    vec2 bbb = max(step(uv, vec2(temp)) * smoothstep(0.0, 1-temp*2, (temp - uv)/(temp)), step(vec2(1-temp), uv) * smoothstep(0.0, 1-temp*2, (uv+temp-1)/temp));

    pageBorder = max(bbb.x, bbb.y);

    uvec2 texelIndex = uvec2(pageIndexF) % uvec2(5);
    texelIndex += mipmapIndex * 5;

    uint color = texelFetch(pageTable, ivec2(texelIndex), 0).r;
    pageIndex = color;
}

void main(){

    int maxLodLevel = int(totalMipmapLevel - 1);

    float dist, lodDistanceFiner, lodDistanceCoarse;
    int mipLevel = mipmapLevelList[getLODLevelRevised(WorldPos, dist, lodDistanceFiner, lodDistanceCoarse)];

    float dist1;
    int mipLevelBorder = mipmapLevelList[getLODLevelBorder(WorldPos, dist1)];

    float ratio = (dist - lodDistanceFiner) / (lodDistanceCoarse - lodDistanceFiner);
    ratio = clamp(ratio, 0, 1);

    float bleedDist = 0.3;
    bleedDist = min(bleedDist, 0.5);
    float aaaaa = max(step(ratio, 0.01), step(1 - bleedDist, ratio));
    aaaaa = clamp(step(ratio, 0.01) + ((ratio + bleedDist - 1) / bleedDist) * step(1 - bleedDist, ratio), 0, 1);

    vec2 uvFiner;
    uint pageIndexFiner;
    float pageBorderFiner;
    getUV(mipLevel, pageIndexFiner, uvFiner, pageBorderFiner);

    vec2 uvCoarse;
    uint pageIndexCoarse;
    float pageBorderCoarse;
    getUV(mipLevelBorder, pageIndexCoarse, uvCoarse, pageBorderCoarse);

    vec3 albedoFinerAutoMipmap = texture(physicalPages, vec3(uvFiner, pageIndexFiner)).rgb;
    vec3 albedoCoarseAutoMipmap = texture(physicalPages, vec3(uvCoarse, pageIndexCoarse)).rgb;
    vec3 albedoBadMipmap = textureLod(physicalPages, vec3(uvCoarse, pageIndexCoarse), 1).rgb;

    float circleAlpha = clamp(((dist - 10) / 10), 0, 1);
    float borders = mix(pageBorderFiner, pageBorderCoarse, aaaaa) * circleAlpha;

    vec3 color = mix(albedoFinerAutoMipmap, albedoCoarseAutoMipmap, aaaaa);
    color = mix(color, albedoBadMipmap, borders);

    FragColor = vec4(vec3(color), 1);
}