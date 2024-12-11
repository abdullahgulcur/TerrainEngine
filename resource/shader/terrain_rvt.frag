//#version 330 core
//
////in vec2 texCoord;
//out vec4 FragColor;
//
////uniform sampler2D mask;
////uniform sampler2D grass;
////uniform sampler2D rock;
////
////uniform vec2 pos;
////uniform int level;
//
//
//void main(){
//
////    float scale = 1 << level;
//////    vec2 uv = pos * scale
////
////    // get mip from level
////    vec3 col0 = textureLod(grass, texCoord * scale, level).rgb;
////    vec3 col1 = textureLod(rock, texCoord * scale, level).rgb;
////    float alpha = textureLod(mask, texCoord, level).r;
////
////    vec3 col = mix(col0, col1, alpha);
//
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
//}
//

#version 460 core
out vec4 FragColor;
in vec2 texCoord;

//uniform sampler2D mask;
uniform sampler2D grass;
//uniform sampler2D rock;

uniform vec2 pos;
uniform int level;

void main() {

    float scale = 1 << level;
    vec2 uv = texCoord * scale * 1;
    vec3 albedo = textureLod(grass, uv, level).rgb;
    FragColor = vec4(albedo , 1.0);
}