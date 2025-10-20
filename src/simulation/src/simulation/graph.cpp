//
// Created by felix on 29.03.25.
//

#include "simulation/graph.hpp"

#include <stdexcept>

using namespace sim;

void graph::connect(pin_t &parentPin, node_t &childNode, pin_sink_t &childPinSink) {
    if (parentPin.type != childPinSink.type) {
        throw std::runtime_error("cannot connect nodes: pin types do not match");
    }
    parentPin.nodes.insert(&childNode);
    childPinSink.pin_value = parentPin.value;
}

void graph::disconnect(pin_t &parentPin, node_t &childNode, pin_sink_t &childPinSink) {
    if (parentPin.type != childPinSink.type) {
        throw std::runtime_error("cannot disconnect nodes: pin types do not match");
    }
    parentPin.nodes.erase(&childNode);
    childPinSink.pin_value = nullptr;
}
