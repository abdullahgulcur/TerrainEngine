#version 460 core

in vec2 WorldPos2D;
in vec3 WorldPos;
out vec4 FragColor;

uniform vec3 cameraPosition;

void main(){

    FragColor = vec4(vec3(0.1, 0.3, 0.8), 1);
}