#version 330 core

in VertexData {
    vec3 color;
} inData;

out vec4 FragColor;

void main() {
    FragColor = vec4(inData.color.xyz, 1.0f);
}