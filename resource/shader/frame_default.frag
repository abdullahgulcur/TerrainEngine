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
    float fogBlend = clamp((depthLinearized - 370.f) / 350.f + 0.5, 0, 1.f);
    return mix(color, vec3(0.9,0.95,1), fogBlend); 
}

void main() {

    vec3 color = texture(frameTexture, texCoord).rgb;
    float depth = texture(frameDepthTexture, texCoord).r;
    depth = linearizeDepth(depth);// / 10000;

    color = getColorAfterFogFilter(color);
    FragColor = vec4(color, 1);//*depth
}