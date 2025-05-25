//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <unordered_map>

#include "node.hpp"

namespace sim {
    class graph {
    public:
        template <typename T>
        static void connect(pin<T> &parentPin, node &childNode, pin_sink<T> &childPinSink);

        template <typename T>
        static void disconnect(pin<T> &parentPin, node &childNode, pin_sink<T> &childPinSink);
    };
}


#endif //GRAPH_H
