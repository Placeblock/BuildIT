#include <iostream>
#include "tree.h"

Gate::Gate() = default;

uint32_t Gate::getInputMask() const {
    return (1 << inputs.size()) - 1;
}

uint32_t Gate::getOutputMask() const {
    return (1 << outputs.size()) - 1;
}

void Gate::setInput(uint8_t index, bool value) {
    if (value) {
        input = input | (1 << index);
    } else {
        input = input & ~(1 << index);
    }
}

void Gate::setOutput(uint8_t index, bool value) {
    if (value) {
        output = output | (1 << index);
    } else {
        output = output & ~(1 << index);
    }
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

void AndGate::update() {
    uint32_t inputMask = getInputMask();
    bool enabled = (input & inputMask) == inputMask && inputMask != 0;
    output = enabled ? getOutputMask() : 0;
}

void NotGate::update() {
    uint32_t inputMask = getInputMask();
    output = ~input & inputMask;
}

OnGate::OnGate() {
    output = ~0u;
}
