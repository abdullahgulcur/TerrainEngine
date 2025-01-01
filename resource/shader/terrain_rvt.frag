#version 460 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D heightmap;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform vec2 pos;
uniform int level;
uniform int blockSize;
uniform vec2 pagePosition;
uniform vec2 physicalTextureSize;

vec2 getUVForHeightmapTexture(){
    return (texCoord / physicalTextureSize) + (pagePosition / physicalTextureSize);
}

void main() {

    float scaleMult = 1.33; // uniform
    float offset = 1.85; // uniform

    float scale = 1 << level;

    vec2 uv = (texCoord + pos) * scale * scaleMult + offset;

    vec3 a0 = textureLod(tex0, uv, level).rgb;
    vec3 a1 = textureLod(tex1, uv, level).rgb;
    float alpha = textureLod(tex2, uv, level).r;

    vec3 albedo = mix(a0, a1, alpha);

    FragColor = vec4(albedo , 1.0);
}