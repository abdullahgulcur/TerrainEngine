#version 460 core

#define HIGH_QUALITY
//#define SHOW_PAGE_BORDERS

layout(location = 0) out vec4 outAlbedo;

in vec2 texCoord;

uniform sampler2DArray palette;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform usampler2D heightmap;
uniform uvec2 blockPosition;
uniform uint level;

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

vec4 bilinearSample(sampler2D texture, vec2 uv) {

    vec2 textureSize = textureSize(texture, 0);

    // Calculate the texel size in texture coordinates
    vec2 texelSize = 1.0 / textureSize;
    
    // Get the four surrounding texel coordinates
    vec2 uv00 = floor(uv * textureSize) / textureSize;
    vec2 uv11 = uv00 + texelSize;
    
    // Calculate the interpolation weights
    vec2 f = fract(uv * textureSize);
    
    // Sample the four texels
    vec4 tex00 = texture2D(texture, uv00);        // Bottom-left
    vec4 tex10 = texture2D(texture, vec2(uv11.x, uv00.y)); // Bottom-right
    vec4 tex01 = texture2D(texture, vec2(uv00.x, uv11.y)); // Top-left
    vec4 tex11 = texture2D(texture, uv11);        // Top-right
    
    // Bilinear interpolation
    vec4 mixX1 = mix(tex00, tex10, f.x); // Interpolate horizontally on the bottom row
    vec4 mixX2 = mix(tex01, tex11, f.x); // Interpolate horizontally on the top row
    return mix(mixX1, mixX2, f.y);       // Interpolate vertically
}

void main() {


    float scale = 1 << level;

    //---------------

    float paddingAmount = 0.05;
    vec2 textureCoordinate = texCoord;
    textureCoordinate = (textureCoordinate * (1 + paddingAmount * 2)) - paddingAmount;

    //---------------

    vec2 uvBase = (blockPosition + textureCoordinate - 0.5) * scale; // AAA   - 0.5

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

#ifndef HIGH_QUALITY
    float hA = bilinearInterpolation(h0, h1, h2, h3, fraction);
#endif

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
#else
	terrainNormal.z = hA - hE;
	terrainNormal.x = hA - hB;
	terrainNormal.y = 1;
	terrainNormal = normalize(terrainNormal);
#endif

    vec3 T = normalize(vec3(2, hB - hC, 0));
    vec3 B = -normalize(cross(T,terrainNormal));
    mat3 tbn = mat3(T, B, terrainNormal);

    //---------------

    float scaleMultiplier = 1.0; // uniform
    vec2 uv = uvBase * scaleMultiplier;

    vec2 uvMacro = uvBase * 0.015;
    vec3 macroVariation = textureLod(tex2, uvMacro, level).rgb;
    vec3 macroVariation1 = textureLod(tex2, uvMacro * 0.25, level).rgb;
    macroVariation += macroVariation1;
    macroVariation *= 0.5;

    float macroScalar = (macroVariation.r + macroVariation.g + macroVariation.b) * 0.33;
    macroScalar = mix(macroScalar, 1, 0.8);

    uint texLevel = level;
    clamp(texLevel, 0, 8);


    vec3 a0;
    vec3 a1;
    vec3 n0;
    vec3 n1;
    float alpha;

    // grass dried
    a0 = textureLod(palette, vec3(uv, 0), texLevel).rgb;
    n0 = textureLod(palette, vec3(uv, 3), texLevel).rgb;

    // grass wild
    a1 = textureLod(palette, vec3(uv, 1), texLevel).rgb;
    n1 = textureLod(palette, vec3(uv, 4), texLevel).rgb;

    alpha = bilinearSample(tex2, uvBase * 0.03).r;

    a0 = mix(a0, a1, alpha);
    n0 = mix(n0, n1, alpha);

    // small rock 
    a1 = clamp(textureLod(palette, vec3(uv*0.5, 2), texLevel).rgb * 1.25, vec3(0), vec3(1));
    n1 = textureLod(palette, vec3(uv*0.5, 5), texLevel).rgb;

    float mmm = 1 - smoothstep(0.9, 1, bilinearSample(tex2, uvBase * 0.02).r * 2);
    alpha = smoothstep(0.4,0.7,dot(n1, vec3(0,1,0))) * mmm;

    a0 = mix(a0, a1, alpha);
    n0 = mix(n0, n1, alpha);

    // rock 0
    vec3 a2 = textureLod(palette, vec3(uv, 2), texLevel).rgb;
    vec3 n2 = textureLod(palette, vec3(uv, 5), texLevel).rgb;

    // rock 1
    vec3 a3 = textureLod(palette, vec3(uv, 6), texLevel).rgb * 0.2;
    vec3 n3 = textureLod(palette, vec3(uv, 7), texLevel).rgb;

    alpha = smoothstep(0.5, 1, bilinearSample(tex2, uvBase * 0.04).r);

    a2 = mix(a2, a3, alpha);
    n2 = mix(n2, n3, alpha);

    //-------------------------

    float slopeBlend = getSlopeBlend(tbn, terrainNormal, n2, 0.95, 0.01, 0.4);
    vec3 albedo = mix(a0, a2, slopeBlend);
    vec3 normal = mix(n0, n2, slopeBlend);
    normal = normalize(normal);

    //normal = vec3(0.5,0.5,1);
    vec3 N = tbn * (normal * 2 - 1);

    float lightPow = 9;
    vec3 lightDir = vec3(-1,-1,-1);
    vec3 L = normalize(-lightDir);
    vec3 radiance = vec3(lightPow);
            
    float NdotL = max(dot(N, L), 0.0);        

    //vec3 viewDir = normalize(cameraPosition - WorldPos);

    //vec3 reflectDir = reflect(-L, N);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.5) * specularity * 0.25;

    vec3 Lo = (albedo / 3.14 + 0) * radiance * NdotL;
    vec3 color = albedo * 0.25 + Lo;

    //---------

#ifdef SHOW_PAGE_BORDERS
    float isBorder = step(paddingAmount, min(texCoord.x, texCoord.y)) * step(max(texCoord.x, texCoord.y), 1 - paddingAmount);
#endif

#ifndef SHOW_PAGE_BORDERS
    outAlbedo = vec4(vec3(color), 1.0); // * vec3(alpha)
#else
    outAlbedo = vec4(color * isBorder, 1.0);
#endif
    
}