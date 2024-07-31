//
// Created by felix on 7/29/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "simulation.h"
#include "node.h"

namespace Graphics {
    class Graphics {

    public:
        Graphics(Sim::Simulation* simulation);
        // The Simulation data has to be stored somehow
        void start();
        void addNode(Node *node);
    private:
        std::vector<Node*> nodes;
        Sim::Simulation* simulation;
    };
}


#endif //BUILDIT_GRAPHICS_H
