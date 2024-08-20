#version 330 core

in TextureData {
    vec2 texCoords;
    vec3 color;
} inData;

out vec4 FragColor;

uniform sampler2D renderedTexture;

void main(){
    FragColor = texture( renderedTexture, inData.texCoords ) * vec4(inData.color.xyz, 1.0);
}