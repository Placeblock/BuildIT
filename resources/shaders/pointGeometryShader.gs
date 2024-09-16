#version 330 core

layout (points) in;
layout (points, max_vertices = 2) out;

in CircleData {
    vec4 color;
    float size;
} inData[];

out VertexData {
    vec4 color;
} outData;

uniform mat4 projection;
uniform float zoom;

void main() {
    outData.color = inData[0].color + vec4(0.2, 0.2, 0.2, 0.0);
    gl_Position = projection * gl_in[0].gl_Position;
    gl_PointSize = inData[0].size*zoom;
    EmitVertex();
    EndPrimitive();
    outData.color = inData[0].color;
    gl_Position = projection * gl_in[0].gl_Position;
    gl_PointSize = (inData[0].size-7)*zoom;
    EmitVertex();
    EndPrimitive();
}