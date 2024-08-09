#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec3 color;

uniform float size;

out CircleData {
    vec3 color;
    float size;
} outData;

void main() {
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    outData.color = color;
    outData.size = size;
}