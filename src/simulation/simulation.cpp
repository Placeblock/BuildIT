//
// Created by felix on 29.03.25.
//

#include "simulation.h"

using namespace Sim;

simulation::simulation(std::unique_ptr<graph> &graph) : graph(std::move(graph)) {
}

void simulation::pollAndUpdate() {
    node *node = this->updateQueue.front();
    this->updateQueue.pop();
    node->update();
    for (const auto &outputPin: node->outputPins) {
        if (!outputPin->pollDirty()) continue;
        for (const auto &childNode: outputPin->nodes) {
            this->updateQueue.push(childNode);
        }
    }
}

void simulation::update(node *node) {
    this->updateQueue.push(node);
}

bool simulation::isEmpty() const {
    return this->updateQueue.empty();
}
