//
// Created by felix on 8/14/24.
//

#include <algorithm>
#include "widget.h"

void Widget::setBufferSize(uint newBufferSize) {
    int delta = newBufferSize - this->bufferSize;
    this->bufferSize = newBufferSize;
    this->parent->updateBufferSizeRecursive(this, delta);
}

void Widget::updateBufferSizeRecursive(Widget* child, int delta) {
    uint newBufferSize = 0;
    for (const auto &item: this->children) {
        newBufferSize += item->bufferSize;
    }
    this->bufferSize = newBufferSize;
    if (this->parent != nullptr) {
        this->parent->updateBufferSizeRecursive(this, delta);
    } else {
        auto iter = std::find_if(this->children.begin(), this->children.end(), [&](const std::unique_ptr<Widget>& w) {
            return child == w.get();
        });
        iter = std::next(iter);
        while (iter != this->children.end()) {
            iter->get()->moveBufferIndexRecursive(delta);
        }
    }
}

void Widget::moveBufferIndexRecursive(int delta) {
    this->bufferIndex += delta;
    for (const auto &item: this->children) {
        item->moveBufferIndexRecursive(delta);
    }
}
