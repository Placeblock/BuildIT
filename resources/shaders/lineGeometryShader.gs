#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

void main() {
    vec4 direction = gl_in[1].gl_Position - gl_in[0].gl_Position;
    direction.xy = normalize(vec2(-direction.y, direction.x)) * 10;
    gl_Position = projection * (gl_in[0].gl_Position + direction);
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - direction);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position + direction);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position - direction);
    EmitVertex();
    EndPrimitive();
}