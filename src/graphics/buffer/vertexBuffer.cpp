//
// Created by felix on 8/30/24.
//

#include <GL/glew.h>
#include <cassert>
#include "vertexBuffer.h"

BufferLayout::BufferLayout() = default;

BufferLayout::BufferLayout(const BufferLayoutElement element) {
    this->addElement(element);
}

void BufferLayout::addElement(const BufferLayoutElement element) {
    this->elements.push_back(element);
}

std::vector<BufferLayoutElement> BufferLayout::getElements() {
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



void Indexed::clear() {
    this->elements.clear();
}

void Indexed::removeElement(Index *element) {
    auto iter = this->elements.begin() + element->index;
    assert(element->index < this->elements.size() && "Tried to remove Index out of bounds.");
    iter = this->elements.erase(iter);
    while (iter != this->elements.end()) {
        (*iter)->index--;
        iter++;
    }
}

Index *Indexed::addElement() {
    this->elements.push_back(std::make_unique<Index>(this->elements.size()));
    return this->elements.back().get();
}



BufferSection *Sectioned::createSection(unsigned int elementIndex, unsigned int size) {
    this->sections.push_back(std::make_unique<BufferSection>(this->sections.size(), elementIndex, size));
    return this->sections.back().get();
}

void Sectioned::addElement(BufferSection *section) {
    section->elements++;
    for (auto sIter = std::next(this->sections.begin(), section->index + 1); sIter != this->sections.end(); sIter++) {
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

bool Sectioned::removeElement(BufferSection *section) {
    auto sectionIter = std::next(this->sections.begin(), section->index);
    section->elements--;
    bool erased = false;
    if (section->elements == 0) {
        sectionIter = this->sections.erase(sectionIter);
        erased = true;
    } else {
        sectionIter++;
    }
    while (sectionIter != this->sections.end()) {
        (*sectionIter)->elementIndex--;
        if (erased) {
            (*sectionIter)->index--;
        }
        sectionIter++;
    }
    return erased;
}