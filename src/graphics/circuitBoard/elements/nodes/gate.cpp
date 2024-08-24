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

//TODO: SHOULDNT THESE BE IN THE NETWORK???
void Gate::onInputConnect(int index, Vertex* vertex) {
    if (vertex->network->inputReference.first != nullptr) {
        Sim::Node* inputNode = vertex->network->inputReference.second.node->simNode.get();
        const auto child = Sim::Reference(this->simNode.get(), inputNode, index);
        const auto parent = Sim::Reference(inputNode, this->simNode.get(), vertex->network->inputReference.second.index);
        this->simulation->connect(parent, child);
    }
    vertex->network->outputReferences[vertex] = Pin(this, index);
}

void Gate::onInputDisconnect(int index, Vertex* vertex) {
    if (vertex->network->inputReference.first != nullptr) {
        Sim::Node* inputNode = vertex->network->inputReference.second.node->simNode.get();
        const auto child = Sim::Reference(this->simNode.get(), inputNode, index);
        const auto parent = Sim::Reference(inputNode, this->simNode.get(), vertex->network->inputReference.second.index);
        this->simulation->disconnect(parent, child);
    }
    vertex->network->outputReferences.erase(vertex);
}

void Gate::onOutputConnect(int index, Vertex* vertex) {
    for (const auto &outRef: vertex->network->outputReferences) {
        Sim::Node* outputNode = outRef.second.node->simNode.get();
        const auto parent = Sim::Reference(this->simNode.get(), outputNode, index);
        const auto child = Sim::Reference(outputNode, this->simNode.get(), outRef.second.index);
        this->simulation->connect(parent, child);
    }
    vertex->network->inputReference = {vertex, Pin(this, index)};
}

void Gate::onOutputDisconnect(int index, Vertex* vertex) {
    for (const auto &outRef: vertex->network->outputReferences) {
        Sim::Node* outputNode = outRef.second.node->simNode.get();
        const auto parent = Sim::Reference(this->simNode.get(), outputNode, index);
        const auto child = Sim::Reference(outputNode, this->simNode.get(), outRef.second.index);
        this->simulation->disconnect(parent, child);
    }
    vertex->network->inputReference = {};
}

Gate::Gate(glm::vec2 pos, InstancedNodeRenderer* renderer, std::string text, Sim::Simulation *simulation, const std::shared_ptr<Sim::Node>& simNode)
    : text(std::move(text)), Node(pos, Gate::calcSize(simNode), simulation, simNode, renderer) {
    this->inputPins = this->calculateInputPins();
    this->outputPins = this->calculateOutputPins();
}

intVec2 Gate::calcSize(const std::shared_ptr<Sim::Node>& simNode) {
    int inputSize = int(simNode->parents.size())+1;
    int outputSize = int(simNode->children.size())+1;
    return {3, std::max(inputSize, outputSize)};
}

void Gate::onMove(glm::vec2 newCell, bool updateSSBO) {
    static_cast<InstancedNodeRenderer*>(this->renderer)->updateInstance(this->cell*32.0f, newCell * 32.0f, updateSSBO);
    Node::onMove(newCell, updateSSBO);
}
