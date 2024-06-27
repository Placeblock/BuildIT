
#include "gate.h"
#include "selection.h"

std::vector<Gate *> Selection::getGates() {
    return this->gates;
}

std::vector<Gate *> Selection::getReversedGates() {
    return std::vector<Gate *>();
}
