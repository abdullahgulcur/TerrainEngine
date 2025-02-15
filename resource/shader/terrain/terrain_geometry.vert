#version 460 core

layout (location = 0) in uvec2 posVert;
layout (location = 1) in uvec2 posInstance;
layout (location = 2) in uint levelInstance;

uniform int blockSize;
uniform mat4 projectionView;
uniform vec3 cameraPosition;
uniform usampler2D heightmap;

out vec2 WorldPos2D;
out vec3 WorldPos;

vec3 getNormal(float dxF, float dyF){

    vec3 normal;
    normal.x = dxF;
    normal.y = 2;
    normal.z = dyF;
    return normalize(normal);
}

vec3 getTangent(float dxF){

    vec3 tangent;
    tangent.x = 2;
    tangent.y = -dxF;
    tangent.z = 0;
    return normalize(tangent);
}

void main(void)
{
    uvec2 worldPos2D = (posVert + posInstance * blockSize) * (1 << levelInstance);
    
    uint height = texelFetch(heightmap, ivec2(worldPos2D), 0).r;
    float heightF = height * 0.03; 

    WorldPos = vec3(worldPos2D.x, heightF, worldPos2D.y);
    WorldPos2D = worldPos2D;

    gl_Position = projectionView * vec4(WorldPos, 1.0);
}