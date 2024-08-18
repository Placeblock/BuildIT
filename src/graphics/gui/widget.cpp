//
// Created by felix on 8/14/24.
//

#include <GL/glew.h>
#include <algorithm>
#include "widget.h"

using namespace GUI;

void Element::setBufferSize(uint newBufferSize) {
    int delta = int(newBufferSize) - int(this->bufferSize);
    this->bufferSize = newBufferSize;
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
        child->setBufferIndex(this->bufferIndex + this->bufferSize);
    } else {
        child->setBufferIndex(this->children.back()->bufferIndex + this->children.back()->getRequiredBufferSpace());
    }
    this->childrenBufferSize += child->getRequiredBufferSpace();
    this->parent->updateBufferSizeRecursive(this, int(child->getRequiredBufferSpace()));
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
    this->vertexBuffer.clear();
    this->texCoordBuffer.clear();
    this->colorBuffer.clear();
    this->textures.clear();
    this->root->render(uintVec2(0, 0), vertexBuffer, texCoordBuffer, colorBuffer, this->textures);

    float vertexData[this->vertexBuffer.size()];
    float texCoordData[this->texCoordBuffer.size()];
    Color colorData[this->colorBuffer.size()];
    std::copy(this->vertexBuffer.begin(), this->vertexBuffer.end(), vertexData);
    std::copy(this->texCoordBuffer.begin(), this->texCoordBuffer.end(), texCoordData);
    std::copy(this->colorBuffer.begin(), this->colorBuffer.end(), colorData);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordData), texCoordData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_DYNAMIC_DRAW);
}

void View::render(Program *program) {
    program->use();

    auto startIter = this->textures.begin();
    auto endIter = this->textures.begin();
    uint start = 0;
    uint count = 0;
    while (++endIter != this->textures.end()) {
        count++;

        if (*endIter != *startIter) {
            glBindTexture(GL_TEXTURE_2D, *startIter);
            glDrawArrays(GL_TRIANGLES, int(start), int(count));

            startIter = endIter;
            start += count;
            count = 0;
        }
    }
}
