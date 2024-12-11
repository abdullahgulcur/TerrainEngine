#version 460 core

out vec4 FragColor;

uniform uint color;

float getValue(int shiftAmount){
    return ((color >> shiftAmount) & 255) / 255.f;
}

void main() {
    
    float r = getValue(24);
    float g = getValue(16);
    float b = getValue(8);
    float a = getValue(0);

    FragColor = vec4(r,g,b,a);
}