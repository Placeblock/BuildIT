//
// Created by felix on 8/18/24.
//

#include "image.h"

using namespace GUI;

Image::Image(View *view, uintVec2 size, unsigned int texture, bool fillParent) : Element(view, size), texture(texture), fillParent(fillParent) {

}

unsigned int Image::calcBufferSize() const {
    return 6;
}

void Image::generateBuffer(std::vector<glm::vec2> &vertices, std::vector<glm::vec2> &texCoords, std::vector<Color> &colors,
                   std::vector<unsigned int> &textures) {
    std::vector<glm::vec2> quadVertices = this->generateQuadVertices();
    vertices.insert(vertices.end(), quadVertices.begin(), quadVertices.end());
    texCoords.insert(texCoords.end(), TEXTURE_COORDS_VEC.begin(), TEXTURE_COORDS_VEC.end());
    colors.insert(colors.end(), TEXTURE_COLORS_VEC.begin(), TEXTURE_COLORS_VEC.end());
    const std::vector<unsigned int> quadTextures(6, this->texture);
    textures.insert(textures.end(), quadTextures.begin(), quadTextures.end());
}

std::vector<glm::vec2> Image::generateQuadVertices() {
    uintVec2 pos = this->getAbsPos();
    std::vector<glm::vec2> vertices{
        {pos.x, pos.y}, {pos.x+this->getSize().x, pos.y}, {pos.x+this->getSize().x, pos.y+this->getSize().y},
        {pos.x+this->getSize().x, pos.y+this->getSize().y}, {pos.x, pos.y+this->getSize().y}, {pos.x, pos.y},
    };
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
