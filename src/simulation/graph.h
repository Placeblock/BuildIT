//
// Created by felix on 29.03.25.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <memory>
#include <unordered_map>

#include "node.h"

namespace Sim {
    class graph {
        std::unordered_map<node *, std::shared_ptr<node> > nodes;

    public:
        static void connect(Pin *parentPin, node *childNode,
                            unsigned int childIndex);

        static void disconnect(Pin *parentPin, node *childNode,
                               unsigned int childIndex);
    };
}


#endif //GRAPH_H
