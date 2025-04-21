//
// Created by felix on 29.03.25.
//

#include "simulation/simulation.hpp"

using namespace Sim;

Simulation::Simulation(std::unique_ptr<Graph> &graph) : graph(std::move(graph)) {
}

void Simulation::pollAndUpdate() {
    Node *node = this->updateQueue.front();
    this->updateQueue.pop();
    node->update([this](const BasePin& pin) {
        for (const auto &childNode: pin.nodes) {
            this->updateQueue.push(childNode);
        }
    });
}

void Simulation::update(Node *node) {
    this->updateQueue.push(node);
}

bool Simulation::isEmpty() const {
    return this->updateQueue.empty();
}
