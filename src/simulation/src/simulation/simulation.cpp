//
// Created by felix on 29.03.25.
//

#include "simulation/simulation.hpp"

using namespace sim;

void simulation::poll_and_update() {
    node *node = this->update_queue.front();
    this->update_queue.pop();
    node->update([this](const base_pin& pin) {
        for (const auto &childNode: pin.nodes) {
            this->update_queue.push(childNode);
        }
    });
}

void simulation::update(node *node) {
    this->update_queue.push(node);
}

bool simulation::is_empty() const {
    return this->update_queue.empty();
}
