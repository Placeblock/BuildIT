//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(glm::vec2 pos, intVec2 size)
    : size(size), Movable(pos) {}

void Node::move(glm::vec2 newPos) {
    Movable::move(newPos);
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

uint8_t Node::getInputPinIndex(glm::vec2 absInputPin) {
    const auto iter = std::find(this->inputPins.begin(), this->inputPins.end(), uintVec2(absInputPin - glm::vec2(this->getPos())));
    return std::distance(this->inputPins.begin(), iter);
}

uint8_t Node::getOutputPinIndex(glm::vec2 absOutputPin) {
    const auto iter = std::find(this->outputPins.begin(), this->outputPins.end(), uintVec2(absOutputPin - glm::vec2(this->getPos())));
    return std::distance(this->outputPins.begin(), iter);
}

Node *Node::getSubject() {
    return this;
}
