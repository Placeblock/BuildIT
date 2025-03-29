//
// Created by felix on 29.03.25.
//

#include "Simulation.h"

using namespace Sim;

Simulation::Simulation(std::unique_ptr<Graph> &graph) : graph(std::move(graph)) {
}

void Simulation::pollAndUpdate() {
    const std::shared_ptr<Node> node = this->updateQueue.front().lock();
    if (node == nullptr) return;
    this->updateQueue.pop();
    node->update();
    for (const auto &outputPin: node->outputPins) {
        if (!outputPin->pollDirty()) continue;
        for (const auto &childNode: this->graph->outputLinks[outputPin.get()]) {
            this->updateQueue.push(childNode);
        }
    }
}

void Simulation::update(const std::weak_ptr<Node> &node) {
    this->updateQueue.push(node);
}

bool Simulation::isEmpty() const {
    return this->updateQueue.empty();
}
