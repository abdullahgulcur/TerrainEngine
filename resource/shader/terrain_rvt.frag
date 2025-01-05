#version 460 core

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outDisplacement;

in vec2 texCoord;

uniform usampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform sampler2D tex8;

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

mat3 getTBN(vec3 normal, vec3 tangent){

    vec3 bitangent = -normalize(cross(tangent, normal));
    return mat3(tangent, bitangent, normal);
}

mat3 getTBN(float dxF, float dyF){

    vec3 normal = getNormal(dxF, dyF);
    vec3 tangent = getTangent(dxF);
    vec3 bitangent = -normalize(cross(tangent, normal));
    return mat3(tangent, bitangent, normal);
}

float getSlopeBlend(mat3 TBN, vec3 macroNormal, vec3 mircoNormal, float slopeBias, float slopeSharpness){

    float worldSpaceSlope = dot(macroNormal, vec3(0,1,0));
    float worldSpaceSlopeBlend = clamp((slopeBias - worldSpaceSlope) / slopeSharpness + 0.5, 0, 1);
    vec3 tangentSpaceNormal = TBN * (mircoNormal * 2 - 1);
    float worldSpaceTextureSlope = dot(tangentSpaceNormal, vec3(0,1,0));
    float slope = mix(worldSpaceSlope, worldSpaceTextureSlope, worldSpaceSlopeBlend);
    float slopeBlend = clamp((slopeBias - slope) / slopeSharpness + 0.5, 0, 1);
    return slopeBlend;
}

vec3 bilinearInterpolation(vec3 val0, vec3 val1, vec3 val2, vec3 val3, vec2 texelGrad){
    vec3 valX0 = mix(val0, val1, texelGrad.x);
    vec3 valX1 = mix(val2, val3, texelGrad.x);
    vec3 valFinal = mix(valX0, valX1, texelGrad.y);
    return valFinal;
}

void main() {

    float scaleMult = 1.0; // uniform
    //float offset = 0.0; // uniform

    float scale = 1 << level;

    //---------------

    vec2 texelPositionF = (pagePosition + texCoord * float(blockSize-1)/blockSize) * blockSize;
    uvec2 texelPositionU = uvec2(texelPositionF);
    vec2 texelGrad = fract(texelPositionF);

    // 2 3
    // 0 1
    uint color0 = texelFetch(tex0, ivec2(texelPositionU), 0).r;
    uint color1 = texelFetch(tex0, ivec2(texelPositionU) + ivec2(1, 0), 0).r;
    uint color2 = texelFetch(tex0, ivec2(texelPositionU) + ivec2(0, 1), 0).r;
    uint color3 = texelFetch(tex0, ivec2(texelPositionU) + ivec2(1, 1), 0).r;

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

    //---------------

    vec3 normal0 = getNormal(dxF0, dyF0);
    vec3 normal1 = getNormal(dxF1, dyF1);
    vec3 normal2 = getNormal(dxF2, dyF2);
    vec3 normal3 = getNormal(dxF3, dyF3);

    vec3 tangent0 = getTangent(dxF0);
    vec3 tangent1 = getTangent(dxF1);
    vec3 tangent2 = getTangent(dxF2);
    vec3 tangent3 = getTangent(dxF3);

    vec3 normalInterpolated = bilinearInterpolation(normal0, normal1, normal2, normal3, texelGrad);
    vec3 tangentInterpolated = bilinearInterpolation(tangent0, tangent1, tangent2, tangent3, texelGrad);
    vec3 bitangentInterpolated = bilinearInterpolation(normal0, normal1, normal2, normal3, texelGrad);

    mat3 tbn = getTBN(normalInterpolated , tangentInterpolated);

    vec3 normalFinal = tbn * vec3(0, 0, 1);

    float alphat; 



    vec2 uvBase = (texCoord + blockPosition) * scale;

    vec2 uv = uvBase * scaleMult;
    vec2 uv0 = uv;
    vec2 uv1 = uv * 0.5;

    vec2 uvMacro = uvBase * 0.3;

    vec2 uvMask_0 = uvBase * 0.03;
    vec2 uvMask_1 = uvBase * 0.05;

    vec3 macroVariation = textureLod(tex2, uvMacro, level).rgb;
    float macroScalar = (macroVariation.r + macroVariation.g + macroVariation.b) * 0.33;
    macroScalar = mix(macroScalar, 1, 0.6);

    vec3 a0 = textureLod(tex3, uv0, level).rgb;
    vec3 a1 = textureLod(tex3, 1 - uv0, level).rgb;

    vec3 n0 = textureLod(tex4, uv0, level).rgb;
    vec3 n1 = textureLod(tex4, 1 - uv0, level).rgb;

    float d0 = textureLod(tex7, uv0, level).r;
    float d1 = textureLod(tex7, 1 - uv0, level).r;

    vec3 a2 = textureLod(tex5, uv1, level).rgb;
    vec3 a3 = textureLod(tex5, 1 - uv1 * 0.75, level).rgb;

    vec3 n2 = textureLod(tex6, uv1, level).rgb;
    vec3 n3 = textureLod(tex6, 1 - uv1 * 0.75, level).rgb;

    float d2 = textureLod(tex8, uv1, level).r;
    float d3 = textureLod(tex8, 1 - uv1 * 0.75, level).r;

    float alpha = textureLod(tex1, uvMask_0, level).r;
    alpha = pow(alpha,2);
    a0 = mix(a0, a1, alpha);
    n0 = mix(n0, n1, alpha);
    d0 = mix(d0, d1, alpha);

    alpha = textureLod(tex1, uvMask_1, level).r;
    alpha = pow(alpha,5);
    a2 = mix(a2, a3, alpha);
    n2 = mix(n2, n3, alpha);
    d2 = mix(d2, d3, alpha);
    alphat = alpha;

    //slope = clamp((0.97 - slope) / 0.03 + 0.5, 0, 1);


    float slopeBlend = getSlopeBlend(tbn, normalFinal, n2, 0.93, 0.1);
//
//    float slope = dot(vec3(0,1,0), normalFinal);
//    float slopeBlend = clamp((0.97 - slope) / 0.03 + 0.5, 0, 1);
//

    vec3 albedo = mix(a0, a2, slopeBlend);
    //vec3 normal = mix(vec3(0.5,0.5,1), n2, slopeBlend);
    vec3 normal = mix(n0, n2, slopeBlend);

    float displacement = mix(d0, d2, slopeBlend);

    albedo *= macroScalar;

    outAlbedo = vec4(albedo, 1.0);
    outNormal = vec4(normal, 1.0);
    outDisplacement = vec4(vec3(displacement), 1.0);

    //FragColor = vec4(vec3(alphat), 1.0);

}