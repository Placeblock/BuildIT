//
// Created by felix on 8/4/24.
//

#include "gate.h"

#include <cmath>
#include <utility>

std::vector<uintVec2> Gate::calculateInputPins() {
    std::vector<uintVec2> cells;
    for (int i = 1; i <= this->simNode->parents.size(); i++) {
        cells.emplace_back(0, i);
    }
    return cells;
}

std::vector<uintVec2> Gate::calculateOutputPins() {
    std::vector<uintVec2> cells;
    for (int i = 1; i <= this->simNode->children.size(); i++) {
        cells.emplace_back(this->size.x, i);
    }
    return cells;
}

Gate::Gate(glm::vec2 pos, InstancedNodeRenderer* renderer, std::string text, const std::shared_ptr<Sim::Node>& simNode)
    : text(std::move(text)), Node(pos, Gate::calcSize(simNode), simNode, renderer) {
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

intVec2 Gate::calcSize(const std::shared_ptr<Sim::Node>& simNode) {
    int inputSize = int(simNode->parents.size())+1;
    int outputSize = int(simNode->children.size())+1;
    return {3, std::max(inputSize, outputSize)};
}

void Gate::onMove(glm::vec2 newCell, bool updateBuffer) {
    static_cast<InstancedNodeRenderer*>(this->renderer)->updateInstance(this->cell*32.0f, newCell * 32.0f, updateBuffer);
    Node::onMove(newCell, updateBuffer);
}
