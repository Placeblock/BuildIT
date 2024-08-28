//
// Created by felix on 8/3/24.
//

#include "node.h"

Node::Node(glm::vec2 cell, intVec2 size, const std::shared_ptr<Sim::Node>& simNode, NodeRenderer* renderer)
    : cell(cell), size(size), simNode(simNode), renderer(renderer) {}

void Node::onMove(glm::vec2 newPos, bool updateBuffer) {
    this->cell = newPos;
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

bool Node::isInside(glm::vec2 checkCell) const {
    return checkCell.x >= this->cell.x && checkCell.y >= this->cell.y
           && checkCell.x <= this->cell.x + float(this->size.x) && checkCell.y <= this->cell.y + float(this->size.y);
}

uint Node::getInputPinIndex(glm::vec2 absInputPin) {
    const auto iter = std::find(this->inputPins.begin(), this->inputPins.end(), uintVec2(absInputPin - glm::vec2(this->cell)));
    return std::distance(this->inputPins.begin(), iter);
}

uint Node::getOutputPinIndex(glm::vec2 absOutputPin) {
    const auto iter = std::find(this->outputPins.begin(), this->outputPins.end(), uintVec2(absOutputPin - glm::vec2(this->cell)));
    return std::distance(this->outputPins.begin(), iter);
}