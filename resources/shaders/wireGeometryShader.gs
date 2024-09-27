#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 8) out;

in VertexData {
    vec4 color;
} inData[];

out VertexData {
    vec4 color;
} outData;

uniform mat4 projection;

void main() {
    outData.color = inData[0].color + vec4(0.2, 0.2, 0.2, 0.0);
    vec4 direction = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 tangent = normalize(vec4(-direction.y, direction.x, direction.zw)) * 5;

    gl_Position = projection * (gl_in[0].gl_Position + tangent);
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - tangent);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position + tangent);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position - tangent);
    EmitVertex();
    EndPrimitive();

    outData.color = inData[0].color;
    gl_Position = projection * (gl_in[0].gl_Position + tangent*0.6);
    EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position - tangent*0.6);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position + tangent*0.6);
    EmitVertex();
    gl_Position = projection * (gl_in[1].gl_Position - tangent*0.6);
    EmitVertex();
}