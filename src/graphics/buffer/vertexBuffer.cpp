//
// Created by felix on 8/30/24.
//

#include <GL/glew.h>
#include <cassert>
#include "vertexBuffer.h"

void BufferLayout::addElement(BufferLayoutElement element) {
    this->elements.push_back(element);
}

std::vector<BufferLayoutElement> BufferLayout::getElements() {
    return this->elements;
}

BufferLayout::BufferLayout() {

}

BufferLayout::BufferLayout(BufferLayoutElement element) {
    this->addElement(element);
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