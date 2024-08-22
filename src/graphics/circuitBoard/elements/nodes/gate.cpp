//
// Created by felix on 8/4/24.
//

#include "gate.h"

#include <cmath>
#include <utility>

std::vector<intVec2> Gate::calculateInputPins() {
    std::vector<intVec2> cells;
    for (int i = 1; i <= this->simNode->parents.size(); i++) {
        cells.emplace_back(this->getCell().x, float(this->getCell().y) + float(i));
    }
    return cells;
}

std::vector<intVec2> Gate::calculateOutputPins() {
    std::vector<intVec2> cells;
    for (int i = 1; i <= this->simNode->children.size(); i++) {
        cells.emplace_back(this->getCell().x + this->size.x, float(this->getCell().y) + float(i));
    }
    return cells;
}

void Gate::onInputConnect(int index, std::shared_ptr<Vertex> vertex) {
    if (vertex->network->inputReference.node != nullptr) {
        const auto child = Sim::Reference(this->simNode, vertex->network->inputReference.node, index);
        const auto parent = Sim::Reference(vertex->network->inputReference.node, this->simNode, vertex->network->inputReference.index);
        this->simulation->connect(parent, child);
    }
    vertex->network->outputReferences.push_back(Sim::Reference(this->simNode, nullptr, index));
}

void Gate::onInputDisconnect(int index, std::shared_ptr<Vertex> vertex) {
    if (vertex->network->inputReference.node != nullptr) {
        const auto child = Sim::Reference(this->simNode, vertex->network->inputReference.node, index);
        const auto parent = Sim::Reference(vertex->network->inputReference.node, this->simNode, vertex->network->inputReference.index);
        this->simulation->disconnect(parent, child);
    }
    vertex->network->outputReferences.erase(
            std::remove(vertex->network->outputReferences.begin(), vertex->network->outputReferences.end(), Sim::Reference(this->simNode,
                                                                                                                           nullptr, index)),
            vertex->network->outputReferences.end());
}

void Gate::onOutputConnect(int index, std::shared_ptr<Vertex> vertex) {
    for (const auto &item: vertex->network->outputReferences) {
        const auto parent = Sim::Reference(this->simNode, item.node, index);
        const auto child = Sim::Reference(item.node, this->simNode, item.index);
        this->simulation->connect(parent, child);
    }
    vertex->network->inputReference = Sim::Reference(this->simNode, nullptr, index);
}

void Gate::onOutputDisconnect(int index, std::shared_ptr<Vertex> vertex) {
    for (const auto &item: vertex->network->outputReferences) {
        const auto parent = Sim::Reference(this->simNode, item.node, index);
        const auto child = Sim::Reference(item.node, this->simNode, item.index);
        this->simulation->disconnect(parent, child);
    }
    vertex->network->inputReference.node = nullptr;
}

Gate::Gate(intVec2 pos, InstancedNodeRenderer<Gate>* renderer, std::string text, Sim::Simulation *simulation, std::shared_ptr<Sim::Node> simNode)
    : renderer(renderer), text(std::move(text)), simulation(simulation), simNode(std::move(simNode)), Node(pos, Gate::calcSize(simNode)) {
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

intVec2 Gate::calcSize(const std::shared_ptr<Sim::Node>& simNode) {
    int inputSize = int(simNode->parents.size())+1;
    int outputSize = int(simNode->children.size())+1;
    return {3, std::max(inputSize, outputSize)};
}

void Gate::onMove(intVec2 newPos, bool updateSSBO) {
    this->renderer->updateInstance(this->pos, newPos, updateSSBO);
    Node::onMove(newPos, updateSSBO);
}
