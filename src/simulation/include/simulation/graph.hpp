//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <unordered_map>

#include "node.hpp"

namespace Sim {
    class Graph {
        std::unordered_map<Node *, std::shared_ptr<Node> > nodes;

    public:
        template <typename T>
        static void connect(Pin<T> &parentPin, Node &childNode, PinSink<T> &childPinSink);

        template <typename T>
        static void disconnect(Pin<T> &parentPin, Node &childNode, PinSink<T> &childPinSink);
    };
}


#endif //GRAPH_H
