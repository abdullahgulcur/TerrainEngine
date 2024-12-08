#version 460 core

layout (location = 0) in vec2 posVert;
layout (location = 1) in vec2 posInstance;
layout (location = 2) in float levelInstance;

uniform int blockSize;
uniform ivec2 terrainSize;

uniform vec2 physicalTextureSize;
uniform mat4 projectionView;
uniform vec3 cameraPosition;

uniform sampler2D pageTable;
uniform sampler2D physicalHeightmap;

out vec2 WorldPos2D;
//out float col;
//out float height;
//out vec2 uv_;

void main(void)
{
    const uint SCALE_BITMASK = 7u;

    WorldPos2D = (posVert + posInstance) * (1 << int(levelInstance));

    vec4 color = texture(pageTable, WorldPos2D / terrainSize).rgba;

    int red = int(color.r * 255);
    int green = int(color.g * 255);
    int blue = int(color.b * 255);
    int alpha = int(color.a * 255);

    int scale = 1 << (alpha & SCALE_BITMASK);

    alpha >>= 3;

    int alpha_remainder_start_x = alpha % (1 << 2);
    alpha >>= 2;

    int alpha_remainder_start_y = alpha;

    int start_x = (red + alpha_remainder_start_x * 256) * blockSize * scale;
    int start_y = (green + alpha_remainder_start_y * 256) * blockSize * scale;

    vec2 offset = WorldPos2D - vec2(start_x, start_y);
    vec2 texturePosNormalized = offset / (scale * blockSize);
    //col = max(offset.x, offset.y);

    int b = int(color.b * 255);
    int pagePosY = b / 16;
    int pagePosX = b % 16;
    vec2 pagePos = vec2(pagePosX, pagePosY);
    vec2 uv = (pagePos + texturePosNormalized) / physicalTextureSize;
    //uv_ = uv;
    float height = texture(physicalHeightmap, uv).r * 100;
    vec3 pos = vec3(WorldPos2D.x, height, WorldPos2D.y);
    gl_Position =  projectionView * vec4(pos, 1.0);
}