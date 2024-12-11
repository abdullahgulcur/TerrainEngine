#version 460 core

in vec2 WorldPos2D;
in float col;
out vec4 FragColor;


uniform int blockSize;
uniform ivec2 terrainSize;
uniform sampler2D pageTable;
uniform sampler2D physicalPages;
uniform vec2 physicalTextureSize;

void main(){

//    int blockSize = 16;
//    int rowsPhysicalTexture = 8;
//    int terrainSize = 4096;
//
//    vec4 color = texture(pageTable, WorldPos2D / terrainSize).rgba;
//
//    int scale = 1 << int(color.a * 255);
//
//    int red = int(color.r * 255);
//    int green = int(color.g * 255);
//    int blue = int(color.b * 255);
//    int alpha = int(color.a * 255);
//
//    int alpha_scale = alpha % (1 << 3);
//    alpha >>= 3;
//
//    int alpha_remainder_start_x = alpha % (1 << 2);
//    alpha >>= 2;
//
//    int alpha_remainder_start_y = alpha;
//
//    int start_x = (red + alpha_remainder_start_x * 256) * blockSize * scale;
//    int start_y = (green + alpha_remainder_start_y * 256) * blockSize * scale;
//
//    vec2 offset = WorldPos2D - vec2(start_x, start_y);
//    vec2 texturePosNormalized = offset / (scale * blockSize);
//    float col = max(texturePosNormalized.x, texturePosNormalized.y);
//
//    int b = int(color.b * 255);
//    int pagePosY = b / 16;
//    int pagePosX = b % 16;
//    vec2 pagePos = vec2(pagePosX, pagePosY);
//    vec2 uv = (pagePos + texturePosNormalized) / rowsPhysicalTexture;
//
//    vec3 ccc = vec3(uv,1);
//    float gridSize = 5;
//    float lineWidth = 1;
//    float valX = step(mod(WorldPos2D.x, gridSize ), lineWidth);
//    float valY = step(mod(WorldPos2D.y, gridSize ), lineWidth);
//    float c = max(valX,valY);
//    c = mix(c, 1, 0.5);


    const uint SCALE_BITMASK = 7u;

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

    vec3 albedo = texture(physicalPages, uv).rgb;

    FragColor = vec4(albedo, 1);

    //FragColor = vec4(c*ccc, 1.0);
}
