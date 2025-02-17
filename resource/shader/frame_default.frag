#version 460 core

uniform sampler2D frameTexture;
uniform sampler2D frameDepthTexture;

out vec4 FragColor;
in vec2 texCoord;

float linearizeDepth(float depth)
{
    float near = 0.1;
    float far = 10000;
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec3 getColorAfterFogFilter(vec3 color){

    float depth = texture(frameDepthTexture, texCoord).r;
    float depthLinearized = linearizeDepth(depth);// / 10000;
    float fogBlend = clamp((depthLinearized - 1000.f) / 1000.f + 0.5, 0, 1.f);
    return mix(color, vec3(0.9,0.95,1), fogBlend); 
}

void main() {

	//vec3 outColor;
    vec3 outColor = texture(frameTexture, texCoord).rgb;


//
//    float FXAA_REDUCE_MIN = (1.0/ 128.0);
//	float FXAA_REDUCE_MUL = (1.0 / 8.0);
//	float FXAA_SPAN_MAX =  8.0;
//	vec2 inverseVP = vec2(1.0 / 1920, 1.0 / 1080);
//	vec3 rgbNW = textureOffset(frameTexture, texCoord, ivec2(-1, -1)).xyz;
//	vec3 rgbNE = textureOffset(frameTexture, texCoord, ivec2(1, -1)).xyz;
//	vec3 rgbSW = textureOffset(frameTexture, texCoord, ivec2(-1, 1)).xyz;
//	vec3 rgbSE = textureOffset(frameTexture, texCoord, ivec2(1, 1)).xyz;
//	vec3 rgbM  = texture(frameTexture, texCoord).xyz;
//	vec3 luma  = vec3(0.299, 0.587, 0.114);
//	float lumaNW = dot(rgbNW, luma);
//	float lumaNE = dot(rgbNE, luma);
//	float lumaSW = dot(rgbSW, luma);
//	float lumaSE = dot(rgbSE, luma);
//	float lumaM  = dot(rgbM,  luma);
//	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
//	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
//	
//	vec2 dir;
//	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
//	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
//	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
//		(0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
//	float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
//	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * inverseVP;
//	
//	vec3 rgbA = (1.0/2.0) * (texture(frameTexture,texCoord + dir * (1.0/3.0 - 0.5)).xyz + texture(frameTexture,texCoord + dir * (2.0/3.0 - 0.5)).xyz);
//	vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (texture(frameTexture,texCoord + dir * (0.0/3.0 - 0.5)).xyz + texture(frameTexture,texCoord + dir * (3.0/3.0 - 0.5)).xyz);
//	
//	float lumaB = dot(rgbB, luma);
//
//
//	if ((lumaB < lumaMin) || (lumaB > lumaMax)){
//		outColor = rgbA;
//	}else{
//		outColor = rgbB;
//	}
//
//	float depth = texture(frameDepthTexture, texCoord).r;
//    depth = linearizeDepth(depth);// / 10000;
	//outColor = getColorAfterFogFilter(outColor);



    FragColor = vec4(outColor, 1);
}