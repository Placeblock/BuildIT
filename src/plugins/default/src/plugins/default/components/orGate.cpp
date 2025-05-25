//
// Created by felix on 23.04.25.
//

#include "../../../../include/plugins/default/components/orGate.hpp"

OrGate::Node::Node(const char inputs) {
    this->inputPins.resize(inputs);
}

void OrGate::Node::update(const std::function<void(const sim::base_pin &pin)> &onUpdated) {
    bool high = false;
    for (const auto &[pin]: this->inputPins) {
        if (pin != nullptr && pin->get_value() == true) {
            high = true;
            break;
        }
    }
    if (this->outputPin->set_value(high)) {
        onUpdated(*this->outputPin);
    }
}
