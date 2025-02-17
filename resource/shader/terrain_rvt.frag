#version 460 core

layout(location = 0) out vec4 outAlbedo;
//layout(location = 1) out vec4 outNormal;
//layout(location = 2) out vec4 outDisplacement;

in vec2 texCoord;

uniform usampler2D heightmap;
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
//uniform uint blockSize;
//uniform uvec2 pagePosition;
//uniform uvec2 pageCounts;
//uniform uint pageSize;

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
    vec3 tangentSpaceNormal = TBN * (mircoNormal * 2 - 1); // yumusatma burada yapilabilir.
    float worldSpaceTextureSlope = dot(tangentSpaceNormal, vec3(0,1,0));
    float slope = mix(worldSpaceSlope, worldSpaceTextureSlope, worldSpaceSlopeBlend);
    float slopeBlend = clamp((slopeBias - slope) / slopeSharpness + 0.5, 0, 1);
    return slopeBlend;
}

float bilinearInterpolation(float val0, float val1, float val2, float val3, vec2 blendFactor){
    float valX0 = mix(val0, val1, blendFactor.x);
    float valX1 = mix(val2, val3, blendFactor.x);
    return mix(valX0, valX1, blendFactor.y);
}

//vec3 bilinearInterpolation(vec3 val0, vec3 val1, vec3 val2, vec3 val3, vec2 texelGrad){
//    vec3 valX0 = mix(val0, val1, texelGrad.x);
//    vec3 valX1 = mix(val2, val3, texelGrad.x);
//    vec3 valFinal = mix(valX0, valX1, texelGrad.y);
//    return valFinal;
//}

float getHeight(ivec2 uv){
    return texelFetch(heightmap, uv, 0).r * 0.03;
}

void main() {

    float scaleMult = 1.0; // uniform
    float offset = 0.0; // uniform

    float scale = 1 << level;

    //---------------

    float bbbb = 0.05 * 1;
    vec2 ttt = texCoord;
    ttt = (ttt * (1 + bbbb * 2)) - bbbb;

    vec2 pos2DF = (blockPosition + ttt - 0.5) * scale; // AAA   - 0.5
    vec2 fraction = fract(pos2DF);

    //float borderDistance = bbbb;

//    float bbb = step(borderDistance, min(texCoord.x, texCoord.y)) * step(max(texCoord.x, texCoord.y), 1 - borderDistance);
//    bbb = mix(bbb, 1, 0.5);

    ivec2 pos2DI = ivec2(pos2DF);

    // Texel Positions
    //    10 11
    // 5  2  3  7
    // 4  0  1  6
    //    8  9

    //   3
    // 1 4 2
    //   0

    float h0 = getHeight(pos2DI);
    float h1 = getHeight(pos2DI + ivec2(1,0));
    float h2 = getHeight(pos2DI + ivec2(0,1));
    float h3 = getHeight(pos2DI + ivec2(1,1));
    float h4 = getHeight(pos2DI + ivec2(-1,0));
    float h5 = getHeight(pos2DI + ivec2(-1,1));
    float h6 = getHeight(pos2DI + ivec2(2,0));
    float h7 = getHeight(pos2DI + ivec2(2,1));
    float h8 = getHeight(pos2DI + ivec2(0,-1));
    float h9 = getHeight(pos2DI + ivec2(1,-1));
    float h10 = getHeight(pos2DI + ivec2(0,2));
    float h11 = getHeight(pos2DI + ivec2(1,2));

    float h4_ = bilinearInterpolation(h0, h1, h2, h3, fraction);
    float h1_ = bilinearInterpolation(h4, h0, h5, h2, fraction);
    float h2_ = bilinearInterpolation(h1, h6, h3, h7, fraction);
    float h0_ = bilinearInterpolation(h8, h9, h0, h1, fraction);
    float h3_ = bilinearInterpolation(h2, h3, h10, h11, fraction);

    vec3 macroNormal;
	macroNormal.z = h0_ - h3_;
	macroNormal.x = h1_ - h2_;
	macroNormal.y = 2;
	macroNormal = normalize(macroNormal);

    vec3 T = normalize(vec3(2, h2 - h1, 0));
    vec3 B = -normalize(cross(T,macroNormal));
    mat3 tbn = mat3(T, B, macroNormal);

    vec2 uvBase = pos2DF;
    //uvBase = uvBase * 1.5 - 0.05;

    vec2 uv = uvBase * scaleMult;
    vec2 uv0 = uv;
    vec2 uv1 = uv * 1;
    vec2 uv2 = uv;

    vec2 uvMacro = uvBase * 0.015;

    vec2 uvMask_0 = uvBase * 0.003;
    vec2 uvMask_1 = uvBase * 0.005;
    vec2 uvMask_2 = uvBase * 0.0025;
    vec2 uvMask_3 = uvBase * 0.001;

    vec3 macroVariation = textureLod(tex2, uvMacro, level).rgb;
    vec3 macroVariation1 = textureLod(tex2, uvMacro * 0.25, level).rgb;
    macroVariation += macroVariation1;
    macroVariation *= 0.5;

    float macroScalar = (macroVariation.r + macroVariation.g + macroVariation.b) * 0.33;
    macroScalar = mix(macroScalar, 1, 0.8);

    uint texLevel = level;
    clamp(texLevel, 0, 8);

    vec3 a0 = textureLod(tex3, uv0, texLevel).rgb;
    vec3 a1 = textureLod(tex3, 1 - uv0, texLevel).rgb;
    vec3 a2 = textureLod(tex5, uv1 * 0.35, texLevel).rgb;
    vec3 a3 = textureLod(tex5, 1 - uv1 * 0.3, texLevel).rgb;
    vec3 a4 = textureLod(tex7, uv2, texLevel).rgb;
    vec3 a5 = textureLod(tex7, 1 - uv2, texLevel).rgb;
    
    vec3 n0 = textureLod(tex4, uv0, texLevel).rgb;
    vec3 n1 = textureLod(tex4, 1 - uv0, texLevel).rgb;
    vec3 n2 = textureLod(tex6, uv1 * 0.35, texLevel).rgb;
    vec3 n3 = textureLod(tex6, 1 - uv1 * 0.3, texLevel).rgb;
    vec3 n4 = textureLod(tex8, uv2, texLevel).rgb;
    vec3 n5 = textureLod(tex8, 1 - uv2, texLevel).rgb;

    float alpha = textureLod(tex1, uvMask_0, texLevel).r;
    alpha = pow(alpha,2);
    a0 = mix(a0, a1, alpha);
    n0 = mix(n0, n1, alpha);
    
    alpha = clamp(textureLod(tex1, uvMask_2, texLevel).r * 0.85, 0, 1);
    alpha = pow(alpha,1.2);
    a0 = mix(a0, a4, alpha);
    n0 = mix(n0, n4, alpha);

    alpha = textureLod(tex1, uvMask_1, texLevel).r;
    alpha = pow(alpha,5);
    a2 = mix(a2, a3, alpha);
    n2 = mix(n2, n3, alpha);
    
    //slope = clamp((0.97 - slope) / 0.03 + 0.5, 0, 1);


    float slopeBlend = getSlopeBlend(tbn, macroNormal, n2, 0.93, 0.1);
//
//    float slope = dot(vec3(0,1,0), normalFinal);
//    float slopeBlend = clamp((0.97 - slope) / 0.03 + 0.5, 0, 1);
//

    vec3 albedo = mix(a0, a2, slopeBlend);
    vec3 normal = mix(n0, n2, slopeBlend);
    normal.z = normal.z * scale;
    //normal.z *= smoothstep(0, 1, level * 0.1);
    normal = normalize(normal);
//
//    vec3 albedo = mix(vec3(1), a2, slopeBlend);
//    vec3 normal = mix(vec3(0.5,0.5,1), n2, slopeBlend);
////
    albedo *= macroScalar;
//
//
//
//    //---------
//
    //vec3 albedo = vec3(1,1,1);
    //vec3 N = normal;//tbn * vec3(0,0,1);
    normal = vec3(0.5,0.5,1);
    vec3 N = tbn * (normal * 2 - 1);

    float lightPow = 5;
    vec3 lightDir = vec3(-1,-1,-1);
    vec3 L = normalize(-lightDir);
    vec3 radiance = vec3(lightPow);
            
    float NdotL = max(dot(N, L), 0.0);        

    //vec3 viewDir = normalize(cameraPosition - WorldPos);

    //vec3 reflectDir = reflect(-L, N);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.5) * specularity * 0.25;

    vec3 Lo = (albedo / 3.14 + 0) * radiance * NdotL;
    vec3 color = albedo * 0.3 + Lo;

    //---------


    //vec3 color = vec3(1);
    outAlbedo = vec4(color * 1, 1.0);
    //outNormal = vec4(normal, 1.0);
    //outDisplacement = vec4(vec3(displacement), 1.0);

    //FragColor = vec4(vec3(alphat), 1.0);

}