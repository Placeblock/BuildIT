//
// Created by felix on 7/29/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include <map>
#include "simulation.h"
#include "node.h"
#include "gateNode.h"

namespace Graphics {
    class Graphics {

    public:
        Graphics(Sim::Simulation* simulation);
        // The Simulation data has to be stored somehow
        void start();
        void addNode(GateNode<> *node);
        void addNode(Node *node);
    private:
        std::vector<Node*> nodes;
        std::map<Sim::Node*, Node*> simNodes;
        Sim::Simulation* simulation;
    };
}


#endif //BUILDIT_GRAPHICS_H
