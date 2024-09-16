#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 color;

uniform mat4 projection;

out TextureData {
    vec2 texCoords;
    vec3 color;
} outData;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    outData.texCoords = texCoords;
    outData.color = color.xyz;
}