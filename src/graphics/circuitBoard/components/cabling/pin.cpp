//
// Created by felix on 9/25/24.
//

#include "pin.h"
#include "graphics/circuitBoard/components/nodes/node.h"

Sim::Node *Pin::getInputSimNode() const {
    return this->node->getInputSimNode(index).node;
}

Sim::Node *Pin::getOutputSimNode() const {
    return this->node->getOutputSimNode(index).node;
}

SimNodeData Pin::getInputSimData() const {
    return this->node->getInputSimNode(index);
}

SimNodeData Pin::getOutputSimData() const {
    return this->node->getOutputSimNode(index);
}