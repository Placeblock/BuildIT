#version 330 core

in VertexData {
    vec4 color;
} inData;

out vec4 FragColor;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5) {
        discard;
    }
    FragColor = inData.color;
}