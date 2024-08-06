//
// Created by felix on 8/4/24.
//

#include "gate.h"

std::unordered_set<glm::vec2> Gate::calculatePinCells() {
    std::unordered_set<glm::vec2> cells;

    float iDelta = this->size.y / float(this->inputs + 1);
    for (int i = 1; i <= this->inputs; ++i) {
        int pos = round(i * iDelta);
        cells.insert(glm::vec2(this->cell.x, this->cell.y+pos));
    }
    float oDelta = this->size.y / float(this->outputs + 1);
    for (int i = 1; i <= this->outputs; ++i) {
        int pos = round(i * oDelta);
        cells.insert(glm::vec2(this->cell.x+this->size.x, this->cell.y+pos));
    }

    return cells;
}

void Gate::onInputConnect(int index, std::shared_ptr<Vertex> vertex) {
    if (vertex->network->inputReference.node != nullptr) {
        const auto parent = Sim::Reference(vertex->network->inputReference.node, this->simNode, vertex->network->inputReference.index);
        const auto child = Sim::Reference(this->simNode, vertex->network->inputReference.node, index);
        Sim::connect(parent, child);
    }
}

void Gate::onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) {
    if (vertex->network->inputReference.node != nullptr) {
        const auto parent = Sim::Reference(vertex->network->inputReference.node, this->simNode, vertex->network->inputReference.index);
        const auto child = Sim::Reference(this->simNode, vertex->network->inputReference.node, index);
        Sim::disconnect(parent, child);
    }
}

void Gate::onOutputConnect(int index, std::shared_ptr<Vertex> vertex) {
    for (const auto &item: vertex->network->outputReferences) {
        const auto parent = Sim::Reference(this->simNode, item.node, index);
        const auto child = Sim::Reference(item.node, this->simNode, item.index);
        Sim::connect(parent, child);
    }
}

void Gate::onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) {
    for (const auto &item: vertex->network->outputReferences) {
        const auto parent = Sim::Reference(this->simNode, item.node, index);
        const auto child = Sim::Reference(item.node, this->simNode, item.index);
        Sim::disconnect(parent, child);
    }
}
