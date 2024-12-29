#version 460 core

uniform sampler2D frameTexture;

out vec4 FragColor;
in vec2 texCoord;

void main() {

    vec3 color = texture(frameTexture, texCoord).rgb;
    FragColor = vec4(color, 1);
}