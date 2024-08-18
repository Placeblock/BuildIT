//
// Created by felix on 8/18/24.
//

#include "image.h"

using namespace GUI;

Image::Image(View *gui, uintVec2 size, uint texture, Element* parent) : Element(gui, size, parent), texture(texture) {

}

uint Image::calcBufferSize() const {
    return 4;
}

void Image::render(uintVec2 pos, std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                   std::vector<uint> &textures) {
    std::vector<float> quadVertices = this->generateQuadVertices(pos);
    vertices.insert(vertices.end(), quadVertices.begin(), quadVertices.end());
    texCoords.insert(texCoords.end(), FULL_TEXTURE_COORDS.begin(), FULL_TEXTURE_COORDS.end());
    colors.insert(colors.end(), FULL_TEXTURE_COLORS.begin(), FULL_TEXTURE_COLORS.end());
    const std::vector<uint> quadTextures(6, this->texture);
    textures.insert(textures.end(), quadTextures.begin(), quadTextures.end());
}

std::vector<float> Image::generateQuadVertices(uintVec2 pos) {
    std::vector<float> vertices(12);
    vertices[0] = float(); vertices[1] = float(pos.y); vertices[2] = float(pos.x+this->size.x);
    vertices[3] = float(pos.y); vertices[4] = float(pos.x+this->size.x); vertices[5] = float(pos.y+this->size.y);
    vertices[6] = float(pos.x+this->size.x); vertices[7] = float(pos.y+this->size.y); vertices[8] = float(pos.x);
    vertices[9] = float(pos.y+this->size.y); vertices[10] = float(pos.x); vertices[11] = float(pos.y);
    return vertices;
}