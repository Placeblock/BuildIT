#version 450

layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(255 / 126, 255 / 66, 255 / 245, 1.0);
    outColor = vec4(255, 255, 255, 1.0);
}