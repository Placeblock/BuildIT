//
// Created by felix on 8/14/24.
//

#include <algorithm>
#include "widget.h"

void Widget::setBufferSize(uint newBufferSize) {
    int delta = int(newBufferSize) - int(this->bufferSize);
    this->bufferSize = newBufferSize;
    this->parent->updateBufferSizeRecursive(this, delta);
}

void Widget::updateBufferSizeRecursive(Widget* child, int delta) {
    this->childrenBufferSize += delta;

    auto iter = std::find_if(this->children.begin(), this->children.end(), [&](const std::unique_ptr<Widget>& w) {
        return child == w.get();
    });
    while (++iter != this->children.end()) {
        (*iter)->moveBufferIndexRecursive(delta);
    }

    if (this->parent != nullptr) {
        this->parent->updateBufferSizeRecursive(this, delta);
    }
}

void Widget::moveBufferIndexRecursive(int delta) {
    this->bufferIndex += delta;
    for (const auto &child: this->children) {
        child->moveBufferIndexRecursive(delta);
    }
}

void Widget::addChild(std::unique_ptr<Widget>& child) {
    if (this->children.empty()) {
        child->setBufferIndex(this->bufferIndex + this->bufferSize);
    } else {
        child->setBufferIndex(this->children.back()->bufferIndex + this->children.back()->getRequiredBufferSpace());
    }
    this->childrenBufferSize += child->getRequiredBufferSpace();
    this->parent->updateBufferSizeRecursive(this, int(child->getRequiredBufferSpace()));
    this->children.push_back(std::move(child));
}

void Widget::removeChild(Widget *child) {
    auto iter = std::find_if(this->children.begin(), this->children.end(), [&child](const std::unique_ptr<Widget>& c){
        return c.get() == child;
    });
    this->updateBufferSizeRecursive(iter->get(), -int(child->getRequiredBufferSpace()));
    this->children.erase(iter);
}

void Widget::setBufferIndex(uint index) {
    this->bufferIndex = index;
}
