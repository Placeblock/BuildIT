//
// Created by felix on 23.04.25.
//

#include "plugins/default/orGate.hpp"

OrGate::Node::Node(const char inputs) {
    this->inputPins.resize(inputs);
}

void OrGate::Node::update(const std::function<void(const Sim::BasePin &pin)> &onUpdated) {
    bool high = false;
    for (const auto &[pin]: this->inputPins) {
        if (pin != nullptr && pin->getValue() == true) {
            high = true;
            break;
        }
    }
    if (this->outputPin->setValue(high)) {
        onUpdated(*this->outputPin);
    }
}
