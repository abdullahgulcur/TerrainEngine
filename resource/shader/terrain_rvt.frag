#version 460 core

#define HIGH_QUALITY
//#define SHOW_PAGE_BORDERS

layout(location = 0) out vec4 outAlbedo;

in vec2 texCoord;

uniform sampler2DArray palette;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform usampler2D heightmap;
uniform usampler2D shadowmap;
uniform uvec2 blockPosition;
uniform uint level;

//vec4 bilinearSample(sampler2D texture, vec2 uv) {
//
//    vec2 textureSize = textureSize(texture, 0);
//
//    // Calculate the texel size in texture coordinates
//    vec2 texelSize = 1.0 / textureSize;
//    
//    // Get the four surrounding texel coordinates
//    vec2 uv00 = floor(uv * textureSize) / textureSize;
//    vec2 uv11 = uv00 + texelSize;
//    
//    // Calculate the interpolation weights
//    vec2 f = fract(uv * textureSize);
//    
//    // Sample the four texels
//    vec4 tex00 = texture2D(texture, uv00);        // Bottom-left
//    vec4 tex10 = texture2D(texture, vec2(uv11.x, uv00.y)); // Bottom-right
//    vec4 tex01 = texture2D(texture, vec2(uv00.x, uv11.y)); // Top-left
//    vec4 tex11 = texture2D(texture, uv11);        // Top-right
//    
//    // Bilinear interpolation
//    vec4 mixX1 = mix(tex00, tex10, f.x); // Interpolate horizontally on the bottom row
//    vec4 mixX2 = mix(tex01, tex11, f.x); // Interpolate horizontally on the top row
//    return mix(mixX1, mixX2, f.y);       // Interpolate vertically
//}

float getMacroVariation(vec2 uv, sampler2D tex, float uvMult0, float uvMult1, float uvMult2, float minVal, float maxVal){

    float val0 = texture(tex, uv*uvMult0).r;
    float val1 = texture(tex, uv*uvMult1).r;
    float val2 = texture(tex, uv*uvMult2).r;
    return mix(minVal, maxVal, val0*val1*val2);
}

float getColorBlend(vec2 uv, sampler2D tex, float offset, float frequency, float minVal, float maxVal){

    float val0 = texture(tex, (uv+offset)*frequency).r;
    float val1 = texture(tex, (uv+offset)*frequency*2).r * 0.5;
    float val2 = texture(tex, (uv+offset)*frequency*4).r * 0.25;
    //    float val3 = texture(tex, uv*frequency*8).r * 0.125;
    float total = (val0+val1+val2)*0.75;
    total = (total - 0.5) * 3;

//    float val3 = texture(tex, uv*frequency*8).r * 0.125;
//    float val4 = texture(tex, uv*frequency*16).r * 0.0625;
    float octave = clamp(total, 0, 1);//+val3+val4
    //octave = (octave - 0.2)*1.3;
    return mix(minVal, maxVal, octave);
}

float getSlopeBlendPoor(mat3 TBN, vec3 terrainNormal, float slopeBias, float slopeSharpness){

    float worldSpaceSlope = dot(terrainNormal, vec3(0,1,0));
    return clamp((slopeBias - worldSpaceSlope) / slopeSharpness + 0.5, 0, 1);
}
float getSlopeBlend(mat3 TBN, vec3 terrainNormal, vec3 mircoNormal, float slopeBias, float slopeSharpness, float reduceFactor){

    float worldSpaceSlope = dot(terrainNormal, vec3(0,1,0));
    float worldSpaceSlopeBlend = clamp((slopeBias - worldSpaceSlope) / slopeSharpness + 0.5, 0, 1);
    vec3 tangentSpaceNormal = TBN * (mircoNormal * 2 - 1);
    float worldSpaceTextureSlope = dot(tangentSpaceNormal, vec3(0,1,0));
    return worldSpaceSlopeBlend * smoothstep(reduceFactor,1,worldSpaceTextureSlope);
}

float bilinearInterpolation(float val0, float val1, float val2, float val3, vec2 blendFactor){
    float valX0 = mix(val0, val1, blendFactor.x);
    float valX1 = mix(val2, val3, blendFactor.x);
    return mix(valX0, valX1, blendFactor.y);
}

float getHeight(ivec2 uv){
    return texelFetch(heightmap, uv, 0).r * 0.03;
}


void main() {


    float scale = 1 << level;

    //---------------

    vec2 uvBase = (blockPosition + texCoord - 0.5) * scale; // AAA   - 0.5

    //---------------

    vec2 fraction = fract(uvBase);
    ivec2 pos2DI = ivec2(uvBase);

    //      Texel Positions
    //
    //         10 --- 11
    //         |       |
    //         |   E   |
    //         |       |
    // 5 ----- 2 ----- 3 -------7
    // |       |       |        |
    // |   C   |   A   |    B   |
    // |       |       |        |
    // 4 ----- 0 ----- 1------- 6
    //         |       |        
    //         |   D   |        
    //         |       |        
    //         8 ----- 9


    float h0 = getHeight(pos2DI);
    float h1 = getHeight(pos2DI + ivec2(1,0));
    float h2 = getHeight(pos2DI + ivec2(0,1));
    float h3 = getHeight(pos2DI + ivec2(1,1));

#ifdef HIGH_QUALITY
    float h4 = getHeight(pos2DI + ivec2(-1,0));
    float h5 = getHeight(pos2DI + ivec2(-1,1));
#endif

    float h6 = getHeight(pos2DI + ivec2(2,0));
    float h7 = getHeight(pos2DI + ivec2(2,1));

#ifdef HIGH_QUALITY
    float h8 = getHeight(pos2DI + ivec2(0,-1));
    float h9 = getHeight(pos2DI + ivec2(1,-1));
#endif

    float h10 = getHeight(pos2DI + ivec2(0,2));
    float h11 = getHeight(pos2DI + ivec2(1,2));

    float hA = bilinearInterpolation(h0, h1, h2, h3, fraction);

#ifdef HIGH_QUALITY
    float hD = bilinearInterpolation(h8, h9, h0, h1, fraction);
    float hC = bilinearInterpolation(h4, h0, h5, h2, fraction);
#endif

    float hB = bilinearInterpolation(h1, h6, h3, h7, fraction);
    float hE = bilinearInterpolation(h2, h3, h10, h11, fraction);


    vec3 terrainNormal;

#ifdef HIGH_QUALITY
	terrainNormal.z = hD - hE;
	terrainNormal.x = hC - hB;
	terrainNormal.y = 2;
	terrainNormal = normalize(terrainNormal);

    vec3 T = normalize(vec3(2, hB - hC, 0));
    vec3 B = -normalize(cross(T,terrainNormal));
    mat3 tbn = mat3(T, B, terrainNormal);

#else
	terrainNormal.z = hA - hE;
	terrainNormal.x = hA - hB;
	terrainNormal.y = 1;
	terrainNormal = normalize(terrainNormal);

    vec3 T = normalize(vec3(1, hB - hA, 0));
    vec3 B = -normalize(cross(T,terrainNormal));
    mat3 tbn = mat3(T, B, terrainNormal);
#endif

    
    uint texLevel = level;
    clamp(texLevel, 0, 9);

    vec3 a0;
    vec3 a1;
    vec3 a2;
    vec3 n0;
    vec3 n1;
    vec3 n2;
    float alpha;

    vec3 aaa;
    a0 = textureLod(palette, vec3(uvBase, 2), texLevel).rgb;
    n0 = textureLod(palette, vec3(uvBase, 7), texLevel).rgb;

    float t = getColorBlend(uvBase, tex1, 12345, 0.005, 0.0, 1);

    a1 = textureLod(palette, vec3(uvBase, 1), texLevel).rgb;
    n1 = textureLod(palette, vec3(uvBase, 6), texLevel).rgb;

    a0 = mix(a0, a1, t);
    n0 = mix(n0, n1, t);

    a1 = textureLod(palette, vec3(uvBase, 3), texLevel).rgb;
    n1 = textureLod(palette, vec3(uvBase, 8), texLevel).rgb;

    a2 = textureLod(palette, vec3(uvBase, 0), texLevel).rgb;
    n2 = textureLod(palette, vec3(uvBase, 5), texLevel).rgb;

    t = getColorBlend(uvBase, tex1, 54321, 0.01, 0.0, 1);

    a1 = mix(a1, a2, t);
    n1 = mix(n1, n2, t);

    float temp = getSlopeBlend(tbn, terrainNormal, n1, 0.90, 0.1, 0.25);

    a0 = mix(a0, a1, temp);
    n0 = mix(n0, n1, temp);

    // sand 0
    a1 = textureLod(palette, vec3(uvBase, 4), texLevel).rgb;
    n1 = textureLod(palette, vec3(uvBase, 9), texLevel).rgb;

    float heightBlend = smoothstep(16, 14.5, hA);
    a0 = mix(a0, a1, heightBlend);
    n0 = mix(n0, n1, heightBlend);

    float macro = getMacroVariation(uvBase, tex2, 0.1, 0.01, 0.001, 0.6, 1);
    a0 *= macro;
    
    vec3 albedo = a0;
    vec3 normal = n0;

    // ----------- LIGHTING -----------
    vec3 N = tbn * (normal * 2 - 1);
    float lightPow = 6;
    vec3 lightDir = vec3(-1,-1,-1);
    vec3 L = normalize(-lightDir);
    vec3 radiance = vec3(lightPow);
    float NdotL = max(dot(N, L), 0.0);        
    vec3 Lo = (albedo / 3.14) * radiance * NdotL;// * (1-shadow);
    vec3 color = albedo * 0.2 + Lo;

    // ----------- OUTPUT -----------

#ifndef SHOW_PAGE_BORDERS
    outAlbedo = vec4(vec3(color), 1.0);
#else
    float paddingAmount = 0.05;
    float isBorder = step(paddingAmount, min(texCoord.x, texCoord.y)) * step(max(texCoord.x, texCoord.y), 1 - paddingAmount);
    outAlbedo = vec4(color * isBorder, 1.0);
#endif
    
}