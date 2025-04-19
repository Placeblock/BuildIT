//
// Created by felix on 29.03.25.
//

#include "simulation.hpp"

using namespace Sim;

Simulation::Simulation(std::unique_ptr<Graph> &graph) : graph(std::move(graph)) {
}

void Simulation::pollAndUpdate() {
    Node *node = this->updateQueue.front();
    this->updateQueue.pop();
    node->update();
    for (const auto &outputPin: node->outputPins) {
        if (!outputPin->pollDirty()) continue;
        for (const auto &childNode: outputPin->nodes) {
            this->updateQueue.push(childNode);
        }
    }
}

void Simulation::update(Node *node) {
    this->updateQueue.push(node);
}

bool Simulation::isEmpty() const {
    return this->updateQueue.empty();
}
