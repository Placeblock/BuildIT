#version 330 core

in TextureData {
    vec2 texCoords;
} inData;

out vec4 FragColor;

uniform sampler2D renderedTexture;

void main(){
    FragColor = texture( renderedTexture, inData.texCoords );
}