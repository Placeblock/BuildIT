//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(glm::vec2 pos, intVec2 cellSize) : cellSize(cellSize), pos(pos),
    AABBInteractable(calcBoundingBox(pos, cellSize * 32)) {}

void Node::move(glm::vec2 newPos) {
    Movable::move(newPos);
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

uint8_t Node::getInputPinIndex(glm::vec2 absInputPin) {
    const uintVec2 pin = uintVec2((absInputPin - glm::vec2(this->getPos())) / 32.0f);
    const auto iter = std::find(this->inputPins.begin(), this->inputPins.end(), pin);
    return std::distance(this->inputPins.begin(), iter);
}

uint8_t Node::getOutputPinIndex(glm::vec2 absOutputPin) {
    const uintVec2 pin = uintVec2((absOutputPin - glm::vec2(this->getPos())) / 32.0f);
    const auto iter = std::find(this->outputPins.begin(), this->outputPins.end(), pin);
    return std::distance(this->outputPins.begin(), iter);
}

void Node::onMove(glm::vec2 delta) {
    this->pos += delta;
}

glm::vec2 Node::getPos() const {
    return this->pos;
}
