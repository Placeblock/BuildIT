//
// Created by felix on 29.03.25.
//

#include "node.h"

using namespace Sim;

Pin::Pin(const unsigned int index)
    : index(index) {
}

bool Pin::getValue() const {
    return this->value;
}

void Pin::setValue(const bool value) {
    if (this->value != value) {
        this->dirty = true;
    }
    this->value = value;
}

bool Pin::pollDirty() {
    if (this->dirty) {
        this->dirty = false;
        return true;
    }
    return false;
}

node::node(const bool pure, const unsigned int inputs, const unsigned int outputs)
    : outputPins(outputs), inputPins(inputs), pure(pure) {
    for (unsigned int i = 0; i < outputs; ++i) {
        this->outputPins[i] = std::make_unique<Pin>(i);
    }
}

bool node::getInput(const unsigned int index) const {
    return this->inputPins[index]->getValue();
}

bool node::getOutput(const unsigned int index) const {
    return this->outputPins[index]->getValue();
}

void node::setOutput(const unsigned int index, const bool value) const {
    this->outputPins[index]->setValue(value);
}

AndNode::AndNode(const char inputs) : node(true, inputs, 1) {
}

void AndNode::update() {
    bool high = true;
    for (const auto &inputPin: this->inputPins) {
        if (inputPin->getValue() == false) {
            high = false;
            break;
        }
    }
    for (const auto &outputPin: this->outputPins) {
        outputPin->setValue(high);
    }
}

NotNode::NotNode() : node(true, 1, 1) {
}

void NotNode::update() {
    this->outputPins[0]->setValue(!this->inputPins[0]->getValue());
}

OrNode::OrNode(const char inputs) : node(true, inputs, 1) {
}

void OrNode::update() {
    bool high = false;
    for (const auto &inputPin: this->inputPins) {
        if (inputPin->getValue() == true) {
            high = true;
            break;
        }
    }
    for (const auto &outputPin: this->outputPins) {
        outputPin->setValue(high);
    }
}
