//
// Created by felix on 13.07.24.
//

#include "node.h"
#include <cstddef>
#include <memory>

using namespace Sim;

Node::Node(uint8_t inputs, uint8_t outputs, std::unique_ptr<Updater> updater) : updater(std::move(updater)) {
    this->parents.resize(inputs);
    this->children.resize(outputs);
    this->recalculateInputMask();
    this->recalculateOutputMask();
    this->update();
}

Node::Node(Node &other) : updater(other.updater->clone()), inputNames(other.inputNames), outputNames(other.outputNames) {
    this->parents.resize(other.parents.size());
    this->children.resize(other.children.size());
    this->recalculateInputMask();
    this->recalculateOutputMask();
    this->update();
}

Node::Node(uint8_t inputs, uint8_t outputs, std::unique_ptr<Updater> updater, uint32_t input) : updater(std::move(updater)), input(input) {
    this->parents.resize(inputs);
    this->children.resize(outputs);
    this->recalculateInputMask();
    this->recalculateOutputMask();
    this->update();
}

void Node::setInput(uint8_t index, bool value) {
    this->input = (this->input & ~(1 << index)) | (value << index);
}

bool Node::getInput(uint8_t index) const {
    return this->input & (1 << index);
}

bool Node::getOutput(uint8_t index) const {
    return this->output & (1 << index);
}

void Node::recalculateInputMask() {
    this->inputMask = (1 << this->parents.capacity()) - 1;
}

void Node::recalculateOutputMask() {
    this->outputMask = (1 << this->children.capacity()) - 1;
}

void Node::update() {
    this->output = this->updater->update(this->input, this->inputMask, this->outputMask) & this->inputMask;
}

uint32_t Node::getOutput() const {
    return this->output;
}

uint32_t Node::getInput() const {
    return this->input;
}

void Sim::update(std::queue<Node*>* queue, Node* node) {
    // Copying old output values for checking them later
    uint32_t oldOutput = node->getOutput();
    // Update the Node
    node->update();
    node->updated = true;
    // Update children of changed outputs
    for (size_t i = 0; i < node->children.size(); ++i) {
        for (const auto &child: node->children[i]) {
            if (child.node != nullptr && (child.node->getInput(child.index) != node->getOutput(i) ||
                    (oldOutput ^ node->getOutput()) & (1 << i))) {
                child.node->setInput(child.index, node->getOutput(i));
                queue->push(child.node);
            }
        }
    }
}