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

void Gate::setOutputs(uint8_t size) {
    this->outputs = size;
    this->recalcOutputMask();
}

void Gate::setInputs(uint8_t size) {
    this->inputs = size;
    this->recalcInputMask();
}

Connection* Gate::connect(ConnectionManager* manager, Gate* childGate, uint8_t outputIndex, uint8_t inputIndex) {
    auto* connection = new Connection{this, childGate, outputIndex, inputIndex};
    manager->children[this].emplace_back(connection);
    manager->parents[childGate].emplace_back(connection);
    childGate->setInput(inputIndex, this->getOutput(outputIndex));
    return connection;
}

void Gate::disconnect(ConnectionManager* manager, Connection* connection) {
    auto i = std::find(manager->parents[this].begin(), manager->parents[this].end(), connection);
    manager->parents[this].erase(i);
    auto j = std::find(manager->children[connection->child].begin(), manager->children[connection->child].end(), connection);
    manager->children[connection->child].erase(j);
    delete connection;
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
