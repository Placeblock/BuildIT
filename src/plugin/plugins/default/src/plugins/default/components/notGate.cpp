//
// Created by felix on 23.04.25.
//

#include "../../../../include/plugins/default/components/notGate.hpp"

NotGate::Node::Node() = default;

void NotGate::Node::update(const std::function<void(const sim::base_pin &pin)> &onUpdated) {
    if (this->inputPin.pin == nullptr) {
        this->outputPin->set_value(false);
        return;
    }
    if (this->outputPin->set_value(this->inputPin.pin->get_value())) {
        onUpdated(*this->outputPin);
    }
}
