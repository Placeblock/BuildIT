#version 450

const vec2 vertices[6] = vec2[](
vec2(0, 0),
vec2(1, 1),
vec2(0, 1),
vec2(1, 1),
vec2(0, 0),
vec2(1, 0)
);

layout (std430, set = 0, binding = 1) readonly buffer culled_indices_buffer {
    uint culled_indices[];
};

layout (std430, set = 1, binding = 0) readonly buffer position_data {
    vec4 bounding_boxes[];
};

layout (std430, set = 1, binding = 1) readonly buffer gate_data {
    uint values[];
};

layout (push_constant) uniform ProjectionMatrix {
    mat4 projection_matrix;
};

layout (location = 0) out vec4 outColor;

void main() {
    uint gate_index = culled_indices[gl_InstanceIndex];
    vec2 pos = bounding_boxes[gate_index].xy;
    int data = int(values[gate_index]);

    vec2 box_coord = vertices[gl_VertexIndex].xy * 20 + pos;
    mat3 matrix = mat3(1.0 / 250, 0, 0, 0, 1.0 / 250, 0, -1, -1, 1);
    matrix = mat3(projection_matrix);
    vec3 translated = matrix * vec3(box_coord.xy, 1);

    // 1/250  0      -1
    // 0      1/250  -1
    // 0      0       1

    gl_Position = vec4(translated.xy, 0.2, 1);
    outColor = vec4(255 * data, 100, 0, 1);
}