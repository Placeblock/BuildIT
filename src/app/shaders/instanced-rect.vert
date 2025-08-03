#version 450

const vec2 positions[6] = vec2[](
vec2(0, 0),
vec2(0, 1),
vec2(1, 1),
vec2(0, 0),
vec2(1, 0),
vec2(1, 1)
);

layout (std430, binding = 0) readonly buffer InstancesBuffer {
    vec4 instances[];
};

layout (push_constant) uniform ProjectionMatrix {
    mat3 projection_matrix;
};

void main() {
    vec2 pos = instances[gl_InstanceIndex].xy;
    vec2 size = instances[gl_InstanceIndex].zw;
    vec2 transformed = positions[gl_VertexIndex] * size + pos;
    gl_Position = transformed * projection_matrix;
}