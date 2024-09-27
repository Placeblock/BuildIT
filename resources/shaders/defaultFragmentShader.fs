#version 330 core

in VertexData {
    vec4 color;
} inData;

out vec4 FragColor;

void main() {
    FragColor = inData.color;
}