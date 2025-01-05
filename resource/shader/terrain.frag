#version 460 core

in vec2 WorldPos2D;
in float col;
out vec4 FragColor;
in vec3 WorldPos;

in vec2 UV;
//in mat3 TBN;
in vec3 Normal;
in vec3 Tangent;
//in vec3 TangentViewPos;
//in vec3 TangentFragPos;

uniform int blockSize;
uniform ivec2 terrainSize;
uniform usampler2D pageTable;
uniform sampler2D physicalPages;
uniform sampler2D physicalPagesNormal;
uniform sampler2D physicalPagesDisplacement;
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

float LinearizeDepth(float depth){
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

mat3 getTBN(vec3 normal, vec3 tangent){

    vec3 bitangent = -normalize(cross(tangent, normal));
    return mat3(tangent, bitangent, normal);
}
vec2 parallaxMapping(vec2 uv){ 
    mat3 tbn = getTBN(Normal,Tangent);
    vec3 TangentViewPos = tbn * cameraPosition;
    vec3 TangentFragPos = tbn * WorldPos;

    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    float displacement = texture(physicalPagesDisplacement, uv).r;
    return uv - viewDir.xy * (displacement * 0.002);        
}
//
//vec2 parallaxOcclusionMapping(vec2 texCoords){ 
//    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
//
//    // number of depth layers
//    const float minLayers = 8;
//    const float maxLayers = 32;
//    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
//    // calculate the size of each layer
//    float layerDepth = 1.0 / numLayers;
//    // depth of current layer
//    float currentLayerDepth = 0.0;
//    // the amount to shift the texture coordinates per layer (from vector P)
//    vec2 P = viewDir.xy / viewDir.z * 0.01; 
//    vec2 deltaTexCoords = P / numLayers;
//  
//    // get initial values
//    vec2  currentTexCoords     = texCoords;
//    float currentDepthMapValue = texture(physicalPagesDisplacement, currentTexCoords).r;
//      
//    while(currentLayerDepth < currentDepthMapValue)
//    {
//        // shift texture coordinates along direction of P
//        currentTexCoords -= deltaTexCoords;
//        // get depthmap value at current texture coordinates
//        currentDepthMapValue = texture(physicalPagesDisplacement, currentTexCoords).r;  
//        // get depth of next layer
//        currentLayerDepth += layerDepth;  
//    }
//    
//    // get texture coordinates before collision (reverse operations)
//    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
//
//    // get depth after and before collision for linear interpolation
//    float afterDepth  = currentDepthMapValue - currentLayerDepth;
//    float beforeDepth = texture(physicalPagesDisplacement, prevTexCoords).r - currentLayerDepth + layerDepth;
// 
//    // interpolation of texture coordinates
//    float weight = afterDepth / (afterDepth - beforeDepth);
//    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
//
//    return finalTexCoords;
//}


void main(){

    vec2 uv = getUV();
    //uv = parallaxMapping(uv);

    vec3 albedo = texture(physicalPages, uv).rgb;
    vec3 normal = texture(physicalPagesNormal, uv).rgb;
    float specularity = (albedo.x + albedo.y + albedo.z) * 0.33;
    //float displacement = texture(physicalPagesDisplacement, uv).r;

    ////------------
    //albedo = vec3(1,1,1);

    //vec3 N = TBN * vec3(0,0,1);//normal;
    mat3 tbn = getTBN(Normal,Tangent);
    vec3 N = tbn * (normal * 2 - 1);

    float lightPow = 5;
    vec3 lightDir = vec3(-1,-1,-1);
    vec3 L = normalize(-lightDir);
    vec3 radiance = vec3(lightPow);
            
    float NdotL = max(dot(N, L), 0.0);        

    vec3 viewDir = normalize(cameraPosition - WorldPos);

    vec3 reflectDir = reflect(-L, N);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.5) * specularity * 0.25;

    vec3 Lo = (albedo / 3.14 + spec) * radiance * NdotL;
    vec3 color = albedo * 0.5 + Lo;
    //------------

    //color = vec3(1,1,1);
    FragColor = vec4(color, 1);
}
