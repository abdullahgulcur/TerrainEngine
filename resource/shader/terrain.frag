#version 460 core

in vec2 WorldPos2D;
in float col;
out vec4 FragColor;
in vec3 WorldPos;

in vec2 UV;
in vec3 Normal;
in vec3 Tangent;

uniform int blockSize;
uniform ivec2 terrainSize;
uniform usampler2D pageTable;
uniform sampler2D physicalPages;
//uniform sampler2D physicalPagesNormal;
//uniform sampler2D physicalPagesDisplacement;
uniform vec2 physicalTexturePageCount;
uniform vec3 cameraPosition;

//--------------- COMMON ---------------
uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1 << bits) - 1);
}
//------------------------------

vec2 getUV(inout uint scalee){

    uint color = texture(pageTable, WorldPos2D / terrainSize).r;
    uint scale = 1 << getValue(color, 0, 4);
    scalee = scale;
    uint pageSizeMultiplier = blockSize * scale;
    vec2 pageStartWorld = floor(WorldPos2D / pageSizeMultiplier) * pageSizeMultiplier;
    vec2 offset_world = WorldPos2D - pageStartWorld ;

    float margin = 1.0 / 512;
    float a = margin;
    float b = 1 - margin * 2;

    vec2 posInPageNormalized = a + (offset_world / pageSizeMultiplier) * b;
    uint pagePosX = getValue(color, 8, 4);
    uint pagePosY = getValue(color, 4, 4);
    vec2 pagePos = vec2(pagePosX, pagePosY);
    return (pagePos + posInPageNormalized) / physicalTexturePageCount;
}

mat3 getTBN(vec3 normal, vec3 tangent){

    vec3 bitangent = -normalize(cross(tangent, normal));
    return mat3(tangent, bitangent, normal);
}

void main(){

    uint scale;
    vec2 uv = getUV(scale);
    
    vec2 dx = dFdx(uv * textureSize(physicalPages, 0));
    vec2 dy = dFdy(uv * textureSize(physicalPages, 0));

    float deltaMax = max(dot(dx, dx), dot(dy, dy));
    float lod = 0.5 * log2(deltaMax);

    //lod += -0.5;
//
    vec3 albedo = textureLod(physicalPages, uv, lod).rgb;
    //vec3 albedo = texture(physicalPages, uv).rgb;
    //vec3 normal = textureLod(physicalPagesNormal, uv, lod).rgb;

    //vec3 albedo = texture(physicalPages, uv).rgb;
    //vec3 normal = texture(physicalPagesNormal, uv).rgb;
    //float specularity = (albedo.x + albedo.y + albedo.z) * 0.33;

    ////------------
    //albedo = vec3(1,1,1);

    //mat3 tbn = getTBN(Normal,Tangent);
    //vec3 N = tbn * (normal * 2 - 1);

//    float lightPow = 5;
//    vec3 lightDir = vec3(-1,-1,-1);
//    vec3 L = normalize(-lightDir);
//    vec3 radiance = vec3(lightPow);
            
    //float NdotL = max(dot(N, L), 0.0);        

    //vec3 viewDir = normalize(cameraPosition - WorldPos);

//    vec3 reflectDir = reflect(-L, N);  
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.5) * specularity * 0.25;

//    vec3 Lo = (albedo / 3.14 + spec) * radiance * NdotL;
//    vec3 color = albedo * 0.5 + Lo;
    //------------

    //color = vec3(1,1,1);
    FragColor = vec4(albedo, 1);
}
