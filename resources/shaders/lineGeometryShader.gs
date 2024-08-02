#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 8) out;

out vec3 fColor;

uniform mat4 projection;

void main() {
    fColor = vec3(0.87, 0.45, 0.16);
    vec4 direction = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 tangent = normalize(vec4(-direction.y, direction.x, direction.zw)) * 10;

    gl_Position = projection * (gl_in[0].gl_Position + tangent);
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - tangent);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position + tangent);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position - tangent);
    EmitVertex();
    EndPrimitive();

    fColor = vec3(0.87, 0.2, 0.16);
    gl_Position = projection * (gl_in[0].gl_Position + tangent*0.6);
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - tangent*0.6);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position + tangent*0.6);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position - tangent*0.6);
    EmitVertex();
}