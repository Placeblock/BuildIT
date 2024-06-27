
#include "algorithm.h"
#include "selection.h"

std::set<Gate *> Selection::getGates() {
    return this->gates;
}

std::set<Gate *> Selection::getSorted() {
    std::set<Gate *> reversed;
    // Checks if any outputs go outside the selection
    for (const auto &gate: this->gates) {
        for (const auto &output: gate->outputs) {
            for (const auto &child: output->children) {
                if (!this->gates.contains(child->gate)) {
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
