//
// Created by felix on 29.03.25.
//

#include "simulation/simulation.hpp"

using namespace sim;

void simulation_t::poll_and_update() {
    node_t *node = this->update_queue.front();
    this->updated_nodes.emplace(node);
    this->update_queue.pop();
    node->update([this](const pin_t *pin) {
        for (const auto &childNode : pin->nodes) {
            this->update_queue.push(childNode);
        }
    });
}

void simulation_t::update(node_t *node) {
    this->update_queue.push(node);
}

bool simulation_t::is_empty() const {
    return this->update_queue.empty();
}
