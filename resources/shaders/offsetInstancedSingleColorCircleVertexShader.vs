#version 460 core

layout (location = 0) in vec2 vertex;

uniform float size;
uniform vec3 color;

layout (binding = 1, std430) readonly buffer ssbo1 {
    vec2 offset[];
};

out CircleData {
    vec3 color;
    float size;
} outData;

void main() {
    gl_Position = vec4(offset[gl_InstanceID].xy, 0.0, 1.0);
    outData.color = normalize(color);
    outData.size = size;
}