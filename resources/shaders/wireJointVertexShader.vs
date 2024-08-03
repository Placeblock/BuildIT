#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec3 color;

out VertexData {
    vec3 color;
} outData;

out float size;

void main() {
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    outData.color = color;
    size = 25;
}