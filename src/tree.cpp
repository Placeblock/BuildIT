#include <iostream>
#include "tree.h"
#include <algorithm>
#include <memory>

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
    this->inputMask = (1 << this->inputs.size()) - 1;
}

void Gate::recalcOutputMask() {
    this->outputMask = (1 << this->outputs.size()) - 1;
}

void Gate::addOutput(uint8_t index) {
    if (index > this->outputs.size()) return;
    this->outputs.insert(this->outputs.begin() + index, std::make_shared<OutputPin>(this, index));
    recalcOutputMask();
}

void Gate::addInput(uint8_t index) {
    if (index > this->inputs.size()) return;
    this->inputs.insert(this->inputs.begin() + index, std::make_shared<InputPin>(this, index));
    recalcInputMask();
}

void Gate::removeOutput(uint8_t index) {
    for (const auto &child: this->outputs[index]->children) {
        child->parent = nullptr;
    }
    this->outputs.erase(this->outputs.begin() + index);
    recalcOutputMask();
}

void Gate::removeInput(uint8_t index) {
    if (this->inputs[index]->parent != nullptr) {
        auto children = this->inputs[index]->parent->children;
        auto pin = std::find(children.begin(), children.end(), this->inputs[index]);
        this->inputs[index]->parent->children.erase(pin);
    }
    this->inputs.erase(this->inputs.begin() + index);
    recalcInputMask();
}

void Gate::connect(Gate* gate, uint8_t outputIndex, uint8_t inputIndex) {
    //std::cout << gate->getName() << " : " << static_cast<int16_t>(inputIndex) << ": " << static_cast<int16_t>(gate->inputs[inputIndex]->index) << "\n";
    this->outputs[outputIndex]->children.emplace_back(gate->inputs[inputIndex]);
    gate->inputs[inputIndex]->parent = this->outputs[outputIndex];
    gate->setInput(inputIndex, this->getOutput(outputIndex));
}

void Gate::disconnect(Gate* gate, uint8_t outputIndex, uint8_t inputIndex) {
    auto i = std::find(this->outputs[outputIndex]->children.begin(), this->outputs[outputIndex]->children.end(), gate->inputs[inputIndex]);
    this->outputs[outputIndex]->children.erase(i);
    gate->inputs[inputIndex]->parent = nullptr;
}

void Gate::remove() {
    for (const auto &outputPin: this->outputs) {
        for (const auto &child: outputPin->children) {
            this->disconnect(child->gate, outputPin->index, child->index);
        }
    }
    for (const auto &inputPin: this->inputs) {
        inputPin->parent->gate->disconnect(this, inputPin->parent->index, inputPin->index);
    }
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

Pin::Pin(Gate *gate, uint8_t index) {
    this->gate = gate;
    this->index = index;
}

InputPin::InputPin(Gate *gate, uint8_t index) : Pin(gate, index) {

}

void InputPin::set(bool enabled) {
    this->gate->setInput(this->index, enabled);
}

bool InputPin::get() {
    return this->gate->getInput(this->index);
}

OutputPin::OutputPin(Gate *gate, uint8_t index) : Pin(gate, index) {

}

void OutputPin::set(bool enabled) {
    this->gate->setOutput(this->index, enabled);
}

bool OutputPin::get() {
    return this->gate->getOutput(this->index);
}
