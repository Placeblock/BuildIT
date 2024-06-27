
#include "algorithm/algorithm.h"
#include "selection.h"

std::set<Node *> Selection::getGates() {
    return this->gates;
}

std::set<Node *> Selection::getSorted() {
    std::set<Node *> reversed;
    // Checks if any outputs go outside the selection
    for (const auto &gate: this->gates) {
        for (const auto &output: gate->outputs) {
            for (const auto &child: output->children) {
                if (!this->gates.contains(child->node)) {
                    reversed.insert(gate);
                }
            }
        }
    }
    for (const auto &gate: this->gates) {
        reversed.insert(gate);
    }
    return reversed;
}
