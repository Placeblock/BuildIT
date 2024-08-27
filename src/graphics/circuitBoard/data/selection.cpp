//
// Created by felix on 8/9/24.
//

#include "selection.h"

void Selection::clear() {
    for (const auto &item: this->joints) {
        this->removeVertex(item, false);
    }
    this->joints.clear();
}

void Selection::addVertex(const Joint* joint) {
    this->joints.insert(joint);
    const long index = this->wires->getJointIndex(joint);
    this->renderer->updateVertexColor(int(index), selectedVertexColor);
}

void Selection::removeVertex(const Joint* joint, bool erase) {
    if (erase) {
        this->joints.erase(joint);
    }
    const long index = this->wires->getJointIndex(joint);
    this->renderer->updateVertexColor(int(index), joint->color);
}
