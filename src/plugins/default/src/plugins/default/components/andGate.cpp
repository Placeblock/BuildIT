//
// Created by felix on 23.04.25.
//

#include "../../../../include/plugins/default/components/andGate.hpp"

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

struct Network {

};
struct Settings {
    int inputs;
};
struct Pin {
    int x, y;
};
struct Test {
    int x, y;
    uint8_t rotation;
    Settings settings;
    Network *network;
    std::array<Pin, 5> pins;
    Pin outputPin;
};