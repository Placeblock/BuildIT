
#include "algorithm/algorithm.h"
#include "selection.h"

std::set<Node *> Selection::getNodes() {
    return this->nodes;
}

std::set<Node *> Selection::getSorted() {
    std::set<Node *> reversed;
    // Checks if any outputs go outside the selection
    for (const auto &gate: this->nodes) {
        for (const auto &output: gate->outputs) {
            for (const auto &child: output->children) {
                if (!this->nodes.contains(child->node)) {
                    reversed.insert(gate);
                }
            }
        }
    }
    for (const auto &gate: this->nodes) {
        reversed.insert(gate);
    }
    return reversed;
}
