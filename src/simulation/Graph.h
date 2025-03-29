//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Node.h"

namespace Sim {
    class Graph {
        std::unordered_map<Node *, std::shared_ptr<Node> > nodes;

    public:
        static void connect(Pin *parentPin, Node *childNode,
                            unsigned int childIndex);

        static void disconnect(Pin *parentPin, Node *childNode,
                               unsigned int childIndex);
    };
}


#endif //GRAPH_H
