//
// Created by felix on 29.03.25.
//

#include "simulation/graph.hpp"

#include <stdexcept>

using namespace sim;

void graph::connect_unsafe(base_pin_t &parentPin,
                           node_t &childNode,
                           base_pin_sink_t &childPinSink) {
    if (parentPin.type != childPinSink.type) {
        throw std::runtime_error("cannot connect nodes: pin types do not match");
    }
    parentPin.nodes.insert(&childNode);
    *childPinSink.get_pin_value() = parentPin.get_value_ptr();
}

void graph::disconnect_unsafe(base_pin_t &parentPin,
                              node_t &childNode,
                              base_pin_sink_t &childPinSink) {
    if (parentPin.type != childPinSink.type) {
        throw std::runtime_error("cannot disconnect nodes: pin types do not match");
    }
    parentPin.nodes.erase(&childNode);
    *childPinSink.get_pin_value() = nullptr;
}