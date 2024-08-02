#version 330 core

layout (location = 0) in float vertex;

uniform vec2 cursor;

out VertexData {
    vec3 color;
} outData;

out float size;

void main() {
    gl_Position = vec4(cursor.xy, 0.0, 1.0);
    outData.color = vec3(0.6, 0.6, 0.6);
    size = 20;
}