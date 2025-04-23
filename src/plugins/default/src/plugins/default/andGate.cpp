//
// Created by felix on 23.04.25.
//

#include "plugins/default/andGate.hpp"

AndGate::Node::Node(const char inputs) {
    this->inputPins.resize(inputs);
}

void AndGate::Node::update(const std::function<void(const Sim::BasePin &pin)> &onUpdated) {
    bool high = true;
    for (const auto &[pin]: this->inputPins) {
        if (pin != nullptr && pin->getValue() == false) {
            high = false;
            break;
        }
    }
    if (this->outputPin->setValue(high)) {
        onUpdated(*this->outputPin);
    }
}
