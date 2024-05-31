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

void Gate::update() {

}

void Gate::recalcInputMask() {
    inputMask = (1 << inputs) - 1;
}

void Gate::recalcOutputMask() {
    outputMask = (1 << outputs.size()) - 1;
}

size_t Gate::connect(size_t index, pin_reference *child) {
    if (outputs.size() > index) {
        outputs[index].emplace_back(child);
        recalcOutputMask();
        child->gate->setInput(child->index, getOutput(index));
        return index;
    } else {
        outputs.emplace_back(std::vector<pin_reference*>{child});
        recalcOutputMask();
        child->gate->setInput(child->index, getOutput(outputs.size() - 1));
        return outputs.size() - 1;
    }
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
