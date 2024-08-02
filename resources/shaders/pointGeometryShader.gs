#version 330 core
layout (points) in;
layout (points, max_vertices = 2) out;

out vec3 fColor;

uniform mat4 projection;
uniform float zoom;

void main() {
    fColor = vec3(0.87, 0.6, 0.16);
    gl_Position = projection * gl_in[0].gl_Position;
    gl_PointSize = 25*zoom;
    EmitVertex();
    EndPrimitive();
    fColor = vec3(0.87, 0.2, 0.16);
    gl_Position = projection * gl_in[0].gl_Position;
    gl_PointSize = 17*zoom;
    EmitVertex();
    EndPrimitive();
}