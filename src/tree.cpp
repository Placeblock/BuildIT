#include <iostream>
#include "tree.h"

Gate::Gate() = default;

void Gate::setInput(uint8_t index, bool value) {
    input = (input & ~(1 << index)) | (value << index);
}

void Gate::setOutput(uint8_t index, bool value) {
    output = (output & ~(1 << index)) | (value << index);
}

bool Gate::getInput(uint8_t index) const {
    return input & (1 << index);
}

bool Gate::getOutput(uint8_t index) const {
    return output & (1 << index);
}

void Gate::updateInputs() {
    for (int i = 0; i < inputs.size(); i++) {
        //std::cout << "Parent value of " << i <<  ": " << inputs[i]->gate->getOutput(inputs[i]->index) << "\n";
        setInput(i, inputs[i]->gate->getOutput(inputs[i]->index));
    }
}

void Gate::update() {

}

void Gate::recalcInputMask() {
    inputMask = (1 << inputs.size()) - 1;
}

void Gate::recalcOutputMask() {
    outputMask = (1 << outputs.size()) - 1;
}

void AndGate::update() {
    bool enabled = (input & inputMask) == inputMask;
    output = enabled ? outputMask : 0;
}

void NotGate::update() {
    output = ~input & inputMask;
}

OnGate::OnGate() {
    output = ~0u;
}
