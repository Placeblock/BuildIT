//
// Created by felix on 23.04.25.
//

#include "plugins/default/components/notGate.hpp"

NotGate::Node::Node() = default;

void NotGate::Node::update(const std::function<void(const sim::base_pin &pin)> &onUpdated) {
    if (this->inputPin.handle == nullptr) {
        this->outputPin->set_value(false);
        return;
    }
    if (this->outputPin->set_value(this->inputPin.handle->get_value())) {
        onUpdated(*this->outputPin);
    }
}
