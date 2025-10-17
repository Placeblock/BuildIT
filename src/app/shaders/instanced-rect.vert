#version 450

const vec2 positions[6] = vec2[](
vec2(0, 0),
vec2(1, 1),
vec2(0, 1),
vec2(1, 1),
vec2(0, 0),
vec2(1, 0)
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
    vec2 box_coord = positions[gl_VertexIndex].xy * size + pos;
    mat3 matrix = mat3(1.0 / 250, 0, 0, 0, 1.0 / 250, 0, -1, -1, 1);
    vec3 translated = matrix * vec3(box_coord.xy, 1);
    //mat3 matrix2 = mat3(1);
    gl_Position = vec4(translated.xy, 0.2, 1);
    //gl_Position = vec4(positions[gl_VertexIndex].xy, 0.2, 1);
}

// 1/500 0     -0.5       x        1/500*x + 0*x     - 0.5
// 0     1/500 -0.5   *   y   =    0*x     + 1/500*y - 0.5
// 0     0     1          1        0*x     + 0*y     + 1