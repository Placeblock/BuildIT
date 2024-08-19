//
// Created by felix on 8/14/24.
//

#include <GL/glew.h>
#include <algorithm>
#include "widget.h"

using namespace GUI;

void Element::setBufferSize(int delta) {
    this->parent->updateBufferSizeRecursive(this, delta);
}

void Element::updateBufferSizeRecursive(Element* child, int delta) {
    this->childrenBufferSize += delta;

    auto iter = std::find_if(this->children.begin(), this->children.end(), [&](const std::unique_ptr<Element>& w) {
        return child == w.get();
    });
    while (++iter != this->children.end()) {
        (*iter)->moveBufferIndexRecursive(delta);
    }

    if (this->parent != nullptr) {
        this->parent->updateBufferSizeRecursive(this, delta);
    }
}

void Element::moveBufferIndexRecursive(int delta) {
    this->bufferIndex += delta;
    for (const auto &child: this->children) {
        child->moveBufferIndexRecursive(delta);
    }
}

void Element::addChild(std::unique_ptr<Element>& child) {
    if (this->children.empty()) {
        child->setBufferIndex(this->bufferIndex + this->calcBufferSize());
    } else {
        child->setBufferIndex(this->children.back()->bufferIndex + this->children.back()->getRequiredBufferSpace());
    }
    this->childrenBufferSize += child->getRequiredBufferSpace();
    if (this->parent != nullptr) {
        this->parent->updateBufferSizeRecursive(this, int(child->getRequiredBufferSpace()));
    }
    this->children.push_back(std::move(child));
}

void Element::removeChild(Element *child) {
    auto iter = std::find_if(this->children.begin(), this->children.end(), [&child](const std::unique_ptr<Element>& c){
        return c.get() == child;
    });
    this->updateBufferSizeRecursive(iter->get(), -int(child->getRequiredBufferSpace()));
    this->children.erase(iter);
}

void Element::setBufferIndex(uint index) {
    this->bufferIndex = index;
}

void Element::updateSize(uintVec2 newSize) {
    if (this->size == newSize) return;
    this->size = newSize;
    if (this->parent != nullptr) {
        this->parent->onChildUpdateSize(this);
    }
    for (const auto &child: this->children) {
        child->onParentUpdateSize();
    }
}

View::View() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(3, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(2);
}


void View::regenerateBuffers() {
    const uint space = this->root->getRequiredBufferSpace();
    this->vertexBuffer.clear();
    this->vertexBuffer.reserve(space*2);
    this->texCoordBuffer.clear();
    this->texCoordBuffer.reserve(space*2);
    this->colorBuffer.clear();
    this->colorBuffer.reserve(space*3);
    this->textures.clear();
    this->textures.reserve(space);
    this->root->render(vertexBuffer, texCoordBuffer, colorBuffer, this->textures);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->vertexBuffer.size(), this->vertexBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*this->texCoordBuffer.size(), this->texCoordBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->colorBuffer.size(), this->colorBuffer.data(), GL_DYNAMIC_DRAW);
}

void View::render(Program *program) {
    program->use();
    glBindVertexArray(this->vAO);

    auto startIter = this->textures.begin();
    auto endIter = this->textures.begin();
    int start = 0;
    int count = 1;
    while (++endIter != this->textures.end()) {
        count++;

        if (*endIter != *startIter || endIter == --this->textures.end()) {
            glBindTexture(GL_TEXTURE_2D, *startIter);
            glDrawArrays(GL_TRIANGLES, start, count);

            startIter = endIter;
            start += count;
            count = 1;
        }
    }
}

void View::updateVertices(Element* element, const std::vector<float> &vertices) {
    uint vIndex = element->getBufferIndex();
    const auto iter = this->vertexBuffer.begin() + vIndex*2;
    std::copy(vertices.begin(), vertices.end(), iter);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vIndex*2*sizeof(float), sizeof(float) * vertices.size(), vertices.data());
}

void View::updateColors(Element* element, const std::vector<unsigned char> &colors) {
    uint cIndex = element->getBufferIndex();
    const auto iter = this->colorBuffer.begin() + cIndex*3;
    std::copy(colors.begin(), colors.end(), iter);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, cIndex*3, colors.size(), colors.data());
}
