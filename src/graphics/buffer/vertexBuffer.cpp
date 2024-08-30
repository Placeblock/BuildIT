//
// Created by felix on 8/30/24.
//

#include <GL/glew.h>
#include <cassert>
#include "vertexBuffer.h"

void MultiElementBufferLayout::addElement(BufferLayoutElement element) {
    this->elements.push_back(element);
    this->stride += element.getSize();
}

unsigned int MultiElementBufferLayout::getSize() const {
    unsigned int size = 0;
    for (const auto &element: this->elements) {
        size += element.getSize();
    }
    return size;
}

std::vector<BufferLayoutElement> MultiElementBufferLayout::getElements() {
    return this->elements;
}

unsigned int BufferLayoutElement::getTypeSize(unsigned int type) {
    switch (type) {
        case GL_FLOAT:
            return sizeof(float);
        case GL_UNSIGNED_BYTE:
            return sizeof(char);
        default:
            assert("Invalid BufferLayoutElement type!");
    }
    return 0;
}

unsigned int BufferLayoutElement::getSize() const {
    return this->count * BufferLayoutElement::getTypeSize(this->type);
}

std::vector<BufferLayoutElement> SingleElementBufferLayout::getElements() {
    return std::vector<BufferLayoutElement>{this->element};
}
