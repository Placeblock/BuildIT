//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_GATENODE_H
#define BUILDIT_GATENODE_H


#include "node.h"
#include "simulation/node.h"

namespace Graphics {
    class GateNode : public Node {
    public:
        GateNode(int x, int y, Simulation::Node simNode);
        const int w = 100, h = 100;
        void updateRendered(int lod, Vector2 offset) override;
        void render(int lod) const override;
    protected:
        Simulation::Node simNode;
    };
}


#endif //BUILDIT_GATENODE_H
