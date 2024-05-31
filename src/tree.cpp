#include <iostream>
#include "tree.h"
#include <algorithm>

Gate::Gate() = default;

void Gate::setInput(uint8_t index, bool value) {
    this->input = (this->input & ~(1 << index)) | (value << index);
}

void Gate::setOutput(uint8_t index, bool value) {
    this->output = (this->output & ~(1 << index)) | (value << index);
}

bool Gate::getInput(uint8_t index) const {
    return this->input & (1 << index);
}

bool Gate::getOutput(uint8_t index) const {
    return this->output & (1 << index);
}

void Gate::update() {

}

void Gate::recalcInputMask() {
    this->inputMask = (1 << this->inputs) - 1;
}

void Gate::recalcOutputMask() {
    this->outputMask = (1 << this->outputs) - 1;
}

void Gate::connect(Gate* childGate, uint8_t outputIndex, uint8_t inputIndex) {
    // FREE THIS SHIT LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    auto* parent_ref = new PinReference{this, outputIndex};
    auto* child_ref = new PinReference{childGate, inputIndex};
    auto* parent = new Parent{parent_ref, inputIndex};
    auto* child = new Child{child_ref, outputIndex};
    this->children.
    this->children[outputIndex].emplace_back(child);
    childGate->parents.emplace_back(parent);
    childGate->setInput(inputIndex, this->getOutput(outputIndex));
}

void Gate::setOutputs(uint8_t size) {
    this->outputs = size;
    this->recalcOutputMask();
}

void Gate::setInputs(uint8_t size) {
    this->inputs = size;
    this->recalcInputMask();
}

void Gate::disconnect(Child *child) {
    auto position = std::find(this->children.begin(), this->children.end(), child);
    this->children.erase(position);
    child->reference->gate->parents
}

void AndGate::update() {
    bool enabled = (this->input & this->inputMask) == this->inputMask;
    this->output = enabled ? this->outputMask : 0;
}

void NotGate::update() {
    this->output = ~this->input & this->inputMask;
}

OnGate::OnGate() {
    this->output = ~0u;
}
