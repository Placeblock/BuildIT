//
// Created by felix on 29.03.25.
//

#include "Node.h"

using namespace Sim;

Pin::Pin(Node *node)
    : node(node) {
}

Node::Node(const bool pure, const unsigned int inputs, const unsigned int outputs)
    : outputPins(outputs), inputPins(inputs), pure(pure) {
}

bool Node::getInput(const unsigned int index) const {
    return this->inputPins[index].lock()->value;
}

bool Node::getOutput(const unsigned int index) const {
    return this->outputPins[index]->value;
}

void Node::setOutput(const unsigned int index, const bool value) const {
    this->outputPins[index]->value = value;
}

AndNode::AndNode(const bool pure, const char inputs) : Node(pure, inputs, 1) {
}

void AndNode::update() {
    bool high = true;
    for (const auto &inputPin: this->inputPins) {
        if (inputPin.lock()->value == false) {
            high = false;
            break;
        }
    }
    for (const auto &outputPin: this->outputPins) {
        outputPin->value = high;
    }
}

NotNode::NotNode(const bool pure) : Node(pure, 1, 1) {
}

void NotNode::update() {
    this->outputPins[0]->value = !this->inputPins[0].lock()->value;
}

OrNode::OrNode(const bool pure, const char inputs) : Node(pure, inputs, 1) {
}

void OrNode::update() {
    bool high = false;
    for (const auto &inputPin: this->inputPins) {
        if (inputPin.lock()->value == true) {
            high = true;
            break;
        }
    }
    for (const auto &outputPin: this->outputPins) {
        outputPin->value = high;
    }
}
