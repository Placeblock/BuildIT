#include <iostream>
#include "tree.h"

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

void Gate::connect(uint8_t index, pin_reference *reference) {
    struct child child = {reference, index};
    this->children.emplace_back(child);
    reference->gate->setInput(reference->index, this->getOutput(index));
}

void Gate::setOutputs(uint8_t size) {
    this->outputs = size;
    this->recalcOutputMask();
}

void Gate::setInputs(uint8_t size) {
    this->inputs = size;
    this->recalcInputMask();
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
    this->setOutputs(1);
}
