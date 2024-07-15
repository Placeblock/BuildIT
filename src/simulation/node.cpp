//
// Created by felix on 13.07.24.
//

#include "node.h"
#include <algorithm>
#include <queue>

Simulation::Node::Node(uint8_t inputs, uint8_t outputs) {
    this->parents.resize(inputs);
    this->children.resize(outputs);
    this->recalculateInputMask();
    this->recalculateOutputMask();
}

void Simulation::Node::setInput(uint8_t index, bool value) {
    this->input = (this->input & ~(1 << index)) | (value << index);
}

void Simulation::Node::setOutput(uint8_t index, bool value) {
    this->output = (this->output & ~(1 << index)) | (value << index);
}

bool Simulation::Node::getInput(uint8_t index) const {
    return this->input & (1 << index);
}

bool Simulation::Node::getOutput(uint8_t index) const {
    return this->output & (1 << index);
}

void Simulation::Node::update() {

}

void Simulation::Node::recalculateInputMask() {
    this->inputMask = (1 << this->parents.capacity()) - 1;
}

void Simulation::Node::recalculateOutputMask() {
    this->outputMask = (1 << this->children.capacity()) - 1;
}

void Simulation::connect(Pin parent, Pin child) {
    // Add child to parents children
    child.targetNode->children[child.targetIndex].emplace_back(child);
    // Add parent to children parents
    parent.targetNode->parents.insert(parent.targetNode->parents.begin() + parent.targetIndex, child);
}

void Simulation::disconnect(Pin parent, Pin child) {
    // Remove child from parents children
    for (auto &pin: child.targetNode->children[child.targetIndex]) {
        if (pin.targetNode == child.targetNode) {
            pin.targetNode = nullptr;
        }
    }
    // Remove parent from children parents
    parent.targetNode->parents[parent.targetIndex].targetNode = nullptr;
}

void Simulation::update(std::queue<Simulation::Node*>* queue, struct Simulation::Node *node) {
    // Copying old output values for checking them later
    uint32_t oldOutput = node->output;
    // Update the node
    node->update();
    // Update children of changed outputs
    for (size_t i = 0; i < node->children.size(); ++i) {
        if ((oldOutput ^ node->output) & (1 << i)) {
            for (const auto &child: node->children[i]) {
                child.targetNode->setInput(child.targetIndex, node->getOutput(i));
                queue->push(child.targetNode);
            }
        }
    }
}