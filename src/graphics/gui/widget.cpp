//
// Created by felix on 8/14/24.
//

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
