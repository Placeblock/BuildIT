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
        explicit Graphics(Sim::Simulation* simulation);
        // The Simulation data has to be stored somehow
        void start();
        void addNode(Node *node);
    private:
        Camera2D camera = { 0, 0, 0, 0, 0.0f, 1.0f };
        std::vector<Node*> nodes;
        Sim::Simulation* simulation;

        void updateShaderOffset(Shader &shader, int offsetLoc) const;
    };
}


#endif //BUILDIT_GRAPHICS_H
