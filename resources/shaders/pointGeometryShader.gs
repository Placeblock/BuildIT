#version 330 core
layout (points) in;
layout (points, max_vertices = 2) out;

in VertexData {
    vec3 color;
} inData[];

out VertexData {
    vec3 color;
} outData;

uniform mat4 projection;
uniform float zoom;

void main() {
    outData.color = inData[0].color + vec3(0.2, 0.2, 0.2);
    gl_Position = projection * gl_in[0].gl_Position;
    gl_PointSize = 25*zoom;
    EmitVertex();
    EndPrimitive();
    outData.color = inData[0].color;
    gl_Position = projection * gl_in[0].gl_Position;
    gl_PointSize = 17*zoom;
    EmitVertex();
    EndPrimitive();
}