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



void Indexed::clear() {
    this->elements.clear();
}

void Indexed::removeElement(Index *element) {
    this->elements.erase(this->elements.begin() + element->index);
}

Index *Indexed::addElement(size_t index) {
    this->elements.insert(this->elements.begin() + index, std::make_unique<Index>(index));
    return this->elements[index].get();
}



BufferSection *Sectioned::createSection(unsigned int elementIndex, unsigned int size) {
    this->sections.push_back(std::make_unique<BufferSection>(this->sections.size(), elementIndex, size));
    return this->sections.back().get();
}

void Sectioned::addElement(BufferSection *section) {
    section->elements++;
    auto sIter = std::next(this->sections.begin(), section->index);
    while (++sIter != this->sections.end()) {
        (*sIter)->elementIndex++;
    }
}

void Sectioned::removeSection(BufferSection *section) {
    auto sectionIter = std::next(this->sections.begin(), section->index);
    const uint elements = section->elements;
    sectionIter = this->sections.erase(sectionIter);
    while (sectionIter != this->sections.end()) {
        (*sectionIter)->index--;
        (*sectionIter)->elementIndex -= elements;
        sectionIter++;
    }
}

void Sectioned::clear() {
    this->sections.clear();
}

void Sectioned::removeElement(BufferSection *section) {
    auto sectionIter = std::next(this->sections.begin(), section->index);
    section->elements--;
    if (section->elements == 0) {
        sectionIter = this->sections.erase(sectionIter);
    } else {
        sectionIter++;
    }
    while (sectionIter != this->sections.end()) {
        (*sectionIter)->elementIndex--;
        sectionIter++;
    }
}