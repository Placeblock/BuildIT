#include <iostream>
#include "algorithm.h"
#include <algorithm>
#include <memory>

Node::Node() = default;

void Node::setInput(uint8_t index, bool value) {
    this->input = (this->input & ~(1 << index)) | (value << index);
}

void Node::setOutput(uint8_t index, bool value) {
    this->output = (this->output & ~(1 << index)) | (value << index);
}

bool Node::getInput(uint8_t index) const {
    return this->input & (1 << index);
}

bool Node::getOutput(uint8_t index) const {
    return this->output & (1 << index);
}

void Node::update() {

}

void Node::recalcInputMask() {
    this->inputMask = (1 << this->inputs.size()) - 1;
}

void Node::recalcOutputMask() {
    this->outputMask = (1 << this->outputs.size()) - 1;
}

void Node::addOutput(uint8_t index) {
    if (index > this->outputs.size()) return;
    this->outputs.insert(this->outputs.begin() + index, std::make_shared<OutputPin>(this, index));
    this->recalcOutputMask();
}

void Node::addInput(uint8_t index) {
    if (index > this->inputs.size()) return;
    this->inputs.insert(this->inputs.begin() + index, std::make_shared<InputPin>(this, index));
    this->recalcInputMask();
}

void Node::removeOutput(uint8_t index) {
    for (const auto &child: this->outputs[index]->children) {
        child->parent = nullptr;
    }
    this->outputs.erase(this->outputs.begin() + index);
    this-> recalcOutputMask();
}

void Node::removeInput(uint8_t index) {
    if (this->inputs[index]->parent != nullptr) {
        auto children = this->inputs[index]->parent->children;
        auto pin = std::find(children.begin(), children.end(), this->inputs[index]);
        this->inputs[index]->parent->children.erase(pin);
    }
    this->inputs.erase(this->inputs.begin() + index);
    this->recalcInputMask();
}

void Node::connect(Node* node, uint8_t outputIndex, uint8_t inputIndex) {
    //std::cout << node->getName() << " : " << static_cast<int16_t>(inputIndex) << ": " << static_cast<int16_t>(node->inputs[inputIndex]->index) << "\n";
    this->outputs[outputIndex]->children.emplace_back(node->inputs[inputIndex]);
    node->inputs[inputIndex]->parent = this->outputs[outputIndex];
    node->setInput(inputIndex, this->getOutput(outputIndex));
}

void Node::disconnect(Node* node, uint8_t outputIndex, uint8_t inputIndex) {
    auto i = std::find(this->outputs[outputIndex]->children.begin(), this->outputs[outputIndex]->children.end(), node->inputs[inputIndex]);
    this->outputs[outputIndex]->children.erase(i);
    node->inputs[inputIndex]->parent = nullptr;
}

void Node::remove() {
    for (const auto &outputPin: this->outputs) {
        for (const auto &child: outputPin->children) {
            this->disconnect(child->node, outputPin->index, child->index);
        }
    }
    for (const auto &inputPin: this->inputs) {
        inputPin->parent->node->disconnect(this, inputPin->parent->index, inputPin->index);
    }
}

void AndNode::update() {
    bool enabled = (this->input & this->inputMask) == this->inputMask;
    this->output = enabled ? this->outputMask : 0;
}

void NotNode::update() {
    this->output = ~this->input & this->inputMask;
}

OnNode::OnNode() {
    this->output = ~0u;
}

Pin::Pin(Node *node, uint8_t index) {
    this->node = node;
    this->index = index;
}

InputPin::InputPin(Node *node, uint8_t index) : Pin(node, index) {

}

void InputPin::set(bool enabled) {
    this->node->setInput(this->index, enabled);
}

bool InputPin::get() {
    return this->node->getInput(this->index);
}

OutputPin::OutputPin(Node *node, uint8_t index) : Pin(node, index) {

}

void OutputPin::set(bool enabled) {
    this->node->setOutput(this->index, enabled);
}

bool OutputPin::get() {
    return this->node->getOutput(this->index);
}
