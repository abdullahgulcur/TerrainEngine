#version 460 core

layout(location = 0) out vec4 outAlbedo;

in vec2 texCoord;

void main() {
    outAlbedo = vec4(0.5,0.5,0.5,1);
}