#version 460 core
layout (location = 0) in uvec2 posVert;
layout (location = 1) in ivec2 posInstance;
layout (location = 2) in uint levelInstance;

uniform int blockSize;
uniform usampler2D heightmap;

void main()
{
    vec2 worldPos2D = (posVert + posInstance * blockSize) * (1 << levelInstance);
    
    uint height = texelFetch(heightmap, ivec2(worldPos2D), 0).r;
    float heightF = height * 0.01; 

    gl_Position = vec4(worldPos2D.x, heightF, worldPos2D.y, 1.0);
}