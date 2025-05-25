//
// Created by felix on 29.03.25.
//

#include "simulation/graph.hpp"

using namespace sim;

template <typename T>
void graph::connect(pin<T> &parentPin, node &childNode, pin_sink<T> &childPinSink) {
    parentPin->nodes.insert(childNode);
    childPinSink.pin = parentPin;
}

template <typename T>
void graph::disconnect(pin<T> &parentPin, node &childNode, pin_sink<T> &childPinSink) {
    parentPin->nodes.erase(childNode);
    childPinSink.pin = nullptr;
}
