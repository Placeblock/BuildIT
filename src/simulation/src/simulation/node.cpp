//
// Created by felix on 29.03.25.
//

#include "simulation/node.hpp"

using namespace Sim;

template <typename T>
Pin<T>::Pin() = default;

template <typename T>
T Pin<T>::getValue() const {
    return this->value;
}

template <typename T>
bool Pin<T>::setValue(const T value) {
    if (this->value != value) {
        this->value = value;
        return true;
    }
    return false;
}

Node::Node() = default;

AndNode::AndNode(const char inputs) {
    this->inputPins.resize(inputs);
}

void AndNode::update(const std::function<void(const BasePin& pin)>& onUpdated) {
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

NotNode::NotNode() = default;

void NotNode::update(const std::function<void(const BasePin& pin)>& onUpdated) {
    if (this->inputPin.pin == nullptr) {
        this->outputPin->setValue(false);
        return;
    }
    if (this->outputPin->setValue(this->inputPin.pin->getValue())) {
        onUpdated(*this->outputPin);
    }
}

OrNode::OrNode(const char inputs) {
    this->inputPins.resize(inputs);
}

void OrNode::update(const std::function<void(const BasePin& pin)>& onUpdated) {
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
