#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

uniform mat4 projectionView;

out vec2 WorldPos2D;
out vec3 WorldPos;

void main(void)
{
    WorldPos2D = pos * 9999;
    WorldPos = vec3(WorldPos2D.x, 30, WorldPos2D.y);
    gl_Position = projectionView * vec4(WorldPos, 1.0);
}