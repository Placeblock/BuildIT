//
// Created by felix on 29.03.25.
//

#include "graph.h"

using namespace Sim;

void graph::connect(Pin *parentPin, node *childNode,
                    const unsigned int childIndex) {
    parentPin->nodes.insert(childNode);
    childNode->inputPins[childIndex] = parentPin;
}

void graph::disconnect(Pin *parentPin, node *childNode,
                       const unsigned int childIndex) {
    parentPin->nodes.erase(childNode);
    childNode->inputPins[childIndex] = nullptr;
}
