#version 460 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec3 color;

uniform mat4 projection;

layout (binding = 2, std430) readonly buffer ssbo1 {
    vec2 offset[];
};

out VertexData {
    vec3 color;
} outData;

void main() {
    gl_Position = projection * (vec4(vertex.xy, 0.0, 1.0) + vec4(offset[gl_InstanceID].xy, 0.0, 0.0));
    outData.color = color;
}