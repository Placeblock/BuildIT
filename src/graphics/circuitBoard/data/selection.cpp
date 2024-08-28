//
// Created by felix on 8/9/24.
//

#include "selection.h"

void Selection::clear() {
    for (const auto &item: this->joints) {
        this->removeJoint(item);
    }
    this->joints.clear();
}

void Selection::addJoint(const Joint* joint) {
    this->joints.insert(joint);
    const size_t index = this->jointContainer->getJointIndex(joint);
    this->renderer->updateJointColor(int(index), SELECTED_COLOR);
}

void Selection::removeJoint(const Joint* joint) {
    const size_t index = this->jointContainer->getJointIndex(joint);
    this->renderer->updateJointColor(int(index), joint->color);
    this->joints.erase(joint);
}

void Selection::addWire(const Wire *wire) {
    this->wires.insert(wire);
    const size_t index = this->wireContainer->getWireIndex(wire);
    this->renderer->updateWireColor(int(index), SELECTED_COLOR);
}

void Selection::removeWire(const Wire *wire) {
    const size_t index = this->wireContainer->getWireIndex(wire);
    this->renderer->updateWireColor(int(index), wire->color);
    this->wires.erase(wire);
}

void Selection::addNode(const Node *node) {
    this->nodes.insert(node);
}

void Selection::removeNode(const Node *node) {
    this->nodes.erase(node);
}
