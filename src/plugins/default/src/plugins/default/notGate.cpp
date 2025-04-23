//
// Created by felix on 23.04.25.
//

#include "plugins/default/notGate.hpp"

NotGate::Node::Node() = default;

void NotGate::Node::update(const std::function<void(const Sim::BasePin &pin)> &onUpdated) {
    if (this->inputPin.pin == nullptr) {
        this->outputPin->setValue(false);
        return;
    }
    if (this->outputPin->setValue(this->inputPin.pin->getValue())) {
        onUpdated(*this->outputPin);
    }
}
