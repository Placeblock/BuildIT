//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H

#include "node.hpp"
#include <stdexcept>

namespace sim {
class graph {
public:
    template<typename T>
    static void connect(pin_t<T> &parentPin, node_t &childNode, pin_sink_t<T> &childPinSink) {
        if (parentPin.type != childPinSink.type) {
            throw std::runtime_error("cannot connect nodes: pin types do not match");
        }
        parentPin.nodes.insert(&childNode);
        childPinSink.pin_value = &parentPin.value;
    }

    template<typename T>
    static void disconnect(pin_t<T> &parentPin, node_t &childNode, pin_sink_t<T> &childPinSink) {
        if (parentPin.type != childPinSink.type) {
            throw std::runtime_error("cannot disconnect nodes: pin types do not match");
        }
        parentPin.nodes.erase(&childNode);
        childPinSink.pin_value = nullptr;
    }

    static void connect_unsafe(base_pin_t &parentPin,
                               node_t &childNode,
                               base_pin_sink_t &childPinSink);

    static void disconnect_unsafe(base_pin_t &parentPin,
                                  node_t &childNode,
                                  base_pin_sink_t &childPinSink);
};
}


#endif //GRAPH_H
