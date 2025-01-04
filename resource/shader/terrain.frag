#version 460 core

in vec2 WorldPos2D;
in float col;
out vec4 FragColor;
in vec3 WorldPos;

in vec2 UV;
in mat3 TBN;
in vec3 Normal;

uniform int blockSize;
uniform ivec2 terrainSize;
uniform usampler2D pageTable;
uniform sampler2D physicalPages;
uniform vec2 physicalTexturePageCount;
uniform vec3 cameraPosition;

//--------------- COMMON ---------------
uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1 << bits) - 1);
}
//------------------------------

vec2 getUV(){

    uint color = texture(pageTable, WorldPos2D / terrainSize).r;
    uint scale = 1 << getValue(color, 0, 4);
    uint pageSizeMultiplier = blockSize * scale;
    uint page_start_x_world = getValue(color, 22, 10) * pageSizeMultiplier;
    uint page_start_y_world = getValue(color, 12, 10) * pageSizeMultiplier;
    vec2 offset_world = WorldPos2D - vec2(page_start_x_world, page_start_y_world);
    vec2 posInPageNormalized = 0.001 + (offset_world / pageSizeMultiplier) * 0.998;
    uint pagePosX = getValue(color, 8, 4);
    uint pagePosY = getValue(color, 4, 4);
    vec2 pagePos = vec2(pagePosX, pagePosY);
    return (pagePos + posInPageNormalized) / physicalTexturePageCount;
}

float LinearizeDepth(float depth) 
{
    float near = 0.1;
    float far = 10000;
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec3 getColorAfterFogFilter(vec3 color){

    float depth = LinearizeDepth(gl_FragCoord.z);// / 10000;
    float fogBlend = clamp((depth - 300.f) / 300.f + 0.5, 0, 1.f);
    return mix(color, vec3(0.9,0.95,1), fogBlend); 
}


void main(){

    vec2 uv = getUV();

    vec3 albedo = texture(physicalPages, uv).rgb;

    ////------------
    //albedo = vec3(1,1,1);
    vec3 normal = Normal;
    //vec3 N = TBN * normal;
    vec3 N = normal;

    float lightPow = 3;
    vec3 lightDir = vec3(1,-1,1);
    vec3 L = normalize(-lightDir);
    vec3 radiance = vec3(lightPow);
            
    float NdotL = max(dot(N, L), 0.0);        

    vec3 viewDir = normalize(cameraPosition - WorldPos);

    vec3 Lo = (albedo / 3.14) * radiance * NdotL;
    vec3 color = albedo * 0.3 + Lo;
    //------------

    //color = vec3(1,1,1);
    FragColor = vec4(color, 1);
}
