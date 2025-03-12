#version 460 core

in vec2 WorldPos2D;
in vec3 WorldPos;
out vec4 FragColor;

uniform int blockSize;
//uniform ivec2 terrainSize;
uniform usampler2D pageTable;
uniform sampler2D physicalPages;
uniform vec2 pageCounts;
uniform vec3 cameraPosition;

int getLODLevel(vec2 worldPosition2D) {

    float margin = 0.1;
    float baseDistance = blockSize * (2 - margin);
    float dist = distance(vec2(cameraPosition.x, cameraPosition.z), worldPosition2D);
    
    int lodLevel = int(floor(log2(dist / (baseDistance * 0.5))));
    int maxLodLevel = int(totalMipmapLevel - 1);
    return clamp(lodLevel, 0, maxLodLevel);
}

vec2 getUV(inout uint scalee){

    uint color = texture(pageTable, WorldPos2D / terrainSize).r;

    uint finerColor = getValue(color, 0, 12);


    uint scale = 1 << getValue(finerColor, 0, 4);
    scalee = scale;
    uint pageSizeMultiplier = blockSize * scale;
    vec2 pageStartWorld = floor(WorldPos2D / pageSizeMultiplier) * pageSizeMultiplier;
    vec2 offset_world = WorldPos2D - pageStartWorld ;

    float margin = 1.0 / 512;
    float a = margin;
    float b = 1 - margin * 2;

    vec2 posInPageNormalized = a + (offset_world / pageSizeMultiplier) * b;
    uint pagePosX = getValue(finerColor, 8, 4);
    uint pagePosY = getValue(finerColor, 4, 4);
    vec2 pagePos = vec2(pagePosX, pagePosY);
    return (pagePos + posInPageNormalized) / physicalTexturePageCount;
}

mat3 getTBN(vec3 normal, vec3 tangent){

    vec3 bitangent = -normalize(cross(tangent, normal));
    return mat3(tangent, bitangent, normal);
}


void main(){

//    uint scale;
//    vec2 uv = getUV(scale);
//    vec2 dx = dFdx(uv * textureSize(physicalPages, 0));
//    vec2 dy = dFdy(uv * textureSize(physicalPages, 0));
//    float deltaMax = max(dot(dx, dx), dot(dy, dy));
//    float lod = 0.5 * log2(deltaMax);
//    //lod += -0.5;
//
//    vec3 albedo = textureLod(physicalPages, uv, lod).rgb;
//    float specularity = (albedo.x + albedo.y + albedo.z) * 0.33;
//
//    ////------------
//    //albedo = vec3(1,1,1);
//
//    vec3 N = Normal;
//
//    float lightPow = 5;
//    vec3 lightDir = vec3(-1,-1,-1);
//    vec3 L = normalize(-lightDir);
//    vec3 radiance = vec3(lightPow);
//            
//    float NdotL = max(dot(N, L), 0.0);        
//
//    vec3 viewDir = normalize(cameraPosition - WorldPos);
//
//    vec3 reflectDir = reflect(-L, N);  
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.5) * specularity * 0.1;
//
//    vec3 Lo = spec * radiance * NdotL;
//    vec3 color = albedo + Lo;

    vec3 color = vec3(0,0,0);
    FragColor = vec4(color, 1);
}
