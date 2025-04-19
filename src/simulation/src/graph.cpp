//
// Created by felix on 29.03.25.
//

#include "graph.hpp"

using namespace Sim;

void Graph::connect(Pin *parentPin, Node *childNode,
                    const unsigned int childIndex) {
    parentPin->nodes.insert(childNode);
    childNode->inputPins[childIndex] = parentPin;
}

void Graph::disconnect(Pin *parentPin, Node *childNode,
                       const unsigned int childIndex) {
    parentPin->nodes.erase(childNode);
    childNode->inputPins[childIndex] = nullptr;
}
