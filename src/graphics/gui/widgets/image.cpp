//
// Created by felix on 8/18/24.
//

#include "image.h"

using namespace GUI;

Image::Image(View *view, uintVec2 size, uint texture, bool fillParent) : Element(view, size), texture(texture), fillParent(fillParent) {

}

uint Image::calcBufferSize() const {
    return 6;
}

void Image::generateBuffer(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                   std::vector<uint> &textures) {
    std::vector<float> quadVertices = this->generateQuadVertices();
    vertices.insert(vertices.end(), quadVertices.begin(), quadVertices.end());
    texCoords.insert(texCoords.end(), FULL_TEXTURE_COORDS.begin(), FULL_TEXTURE_COORDS.end());
    colors.insert(colors.end(), FULL_TEXTURE_COLORS.begin(), FULL_TEXTURE_COLORS.end());
    const std::vector<uint> quadTextures(6, this->texture);
    textures.insert(textures.end(), quadTextures.begin(), quadTextures.end());
}

std::vector<float> Image::generateQuadVertices() {
    uintVec2 pos = this->getAbsPos();
    std::vector<float> vertices(12);
    vertices[0] = float(pos.x); vertices[1] = float(pos.y); vertices[2] = float(pos.x+this->getSize().x);
    vertices[3] = float(pos.y); vertices[4] = float(pos.x+this->getSize().x); vertices[5] = float(pos.y+this->getSize().y);
    vertices[6] = float(pos.x+this->getSize().x); vertices[7] = float(pos.y+this->getSize().y); vertices[8] = float(pos.x);
    vertices[9] = float(pos.y+this->getSize().y); vertices[10] = float(pos.x); vertices[11] = float(pos.y);
    return vertices;
}

void Image::onParentUpdateSize() {
    if (this->fillParent && this->getParent() != nullptr) {
        this->updateSize(this->getParent()->getSize());
    }
}

void Image::updateSize(uintVec2 newSize) {
    Element::updateSize(newSize);
    this->view->updateVertices(this, this->generateQuadVertices());
}
