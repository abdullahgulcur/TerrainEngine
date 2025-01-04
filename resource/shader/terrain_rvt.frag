#version 460 core
out vec4 FragColor;
in vec2 texCoord;

uniform usampler2D heightmap;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform uvec2 blockPosition;
uniform uint level;
uniform uint blockSize;
uniform uvec2 pagePosition;
uniform uvec2 physicalTextureSize;
//
//vec2 getUVForHeightmapTexture(){
//    return (texCoord / physicalTextureSize) + (pagePosition / physicalTextureSize);
//}

//--------------- COMMON ---------------
uint getValue(uint color, int startBit, int bits){
    return (color >> startBit) & ((1 << bits) - 1);
}

void getValues(uint color, inout float dxF, inout float dyF, inout float heightF){

    float heightScale = 0.03; //uniform ?

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
//------------------------------

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

void main() {

    float scaleMult = 1; // uniform
    float offset = 0; // uniform

    float scale = 1 << level;

    //---------------

    vec2 texelPositionF = (pagePosition + texCoord * float(blockSize-1)/blockSize) * blockSize;
    uvec2 texelPositionU = uvec2(texelPositionF);
    vec2 texelGrad = fract(texelPositionF);

    // 2 3
    // 0 1
    uint color0 = texelFetch(heightmap, ivec2(texelPositionU), 0).r;
    uint color1 = texelFetch(heightmap, ivec2(texelPositionU) + ivec2(1, 0), 0).r;
    uint color2 = texelFetch(heightmap, ivec2(texelPositionU) + ivec2(0, 1), 0).r;
    uint color3 = texelFetch(heightmap, ivec2(texelPositionU) + ivec2(1, 1), 0).r;

    float dxF0;
    float dyF0;
    float heightF0;
    getValues(color0, dxF0, dyF0, heightF0);

    float dxF1;
    float dyF1;
    float heightF1;
    getValues(color1, dxF1, dyF1, heightF1);

    float dxF2;
    float dyF2;
    float heightF2;
    getValues(color2, dxF2, dyF2, heightF2);

    float dxF3;
    float dyF3;
    float heightF3;
    getValues(color3, dxF3, dyF3, heightF3);

    vec3 normal0 = getNormal(dxF0, dyF0);
    vec3 normal1 = getNormal(dxF1, dyF1);
    vec3 normal2 = getNormal(dxF2, dyF2);
    vec3 normal3 = getNormal(dxF3, dyF3);

    vec3 normalX0 = mix(normal0, normal1, texelGrad.x);
    vec3 normalX1 = mix(normal2, normal3, texelGrad.x);
    vec3 normalFinal = mix(normalX0, normalX1, texelGrad.y);


    float slope = dot(vec3(0,1,0), normalFinal);
    slope = clamp((0.97 - slope) / 0.03 + 0.5, 0, 1);

//    slope *= 2;
//    slope = clamp(slope, 0, 1);

    //---------------

    vec2 uv = (texCoord + vec2(blockPosition)) * scale * scaleMult + offset;
    vec2 uv0 = (texCoord + vec2(blockPosition)) * scale * 0.02;

    vec3 a0 = textureLod(tex0, uv, level).rgb;
    vec3 a3 = textureLod(tex3, uv, level).rgb;

    float alpha = textureLod(tex2, uv0, level).r;
    a0 = mix(a0, a3, alpha);

    vec3 a1 = textureLod(tex1, uv, level).rgb;
    //float alpha = textureLod(tex2, uv, level).r;

    vec3 albedo = mix(a0, a1, slope);

    FragColor = vec4(albedo, 1.0);
}