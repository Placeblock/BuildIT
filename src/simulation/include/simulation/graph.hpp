//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H

#include "node.hpp"

namespace sim {
class graph {
public:
    static void connect(pin_t &parentPin, node_t &childNode, pin_sink_t &childPinSink);

    static void disconnect(pin_t &parentPin, node_t &childNode, pin_sink_t &childPinSink);
};
}


#endif //GRAPH_H
