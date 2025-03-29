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
        std::unordered_map<Node *, std::unique_ptr<Node> > nodes;

        std::unordered_map<std::weak_ptr<Pin>, std::unordered_set<std::weak_ptr<Pin> > > outputLinks;
        std::unordered_map<std::weak_ptr<Pin>, std::weak_ptr<Pin> > inputLinks;

    public:
        Graph() : outputLinks({}), inputLinks({}) {
        }

        void connect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Pin> &childPin);

        void disconnect(const std::weak_ptr<Pin> &parentPin, const std::weak_ptr<Pin> &childPin);
    };
}


#endif //GRAPH_H
