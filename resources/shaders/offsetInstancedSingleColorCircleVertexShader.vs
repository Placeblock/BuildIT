#version 430 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 offset;

uniform float size;
uniform vec4 color;

out CircleData {
    vec4 color;
    float size;
} outData;

void main() {
    gl_Position = vec4(offset.xy, 0.0, 1.0);
    outData.color = normalize(color);
    outData.size = size;
}
