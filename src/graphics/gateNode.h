//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_GATENODE_H
#define BUILDIT_GATENODE_H


#include "node.h"
#include "simulation/node.h"

namespace Graphics {
    template <typename N = Sim::Node> class GateNode : public Node {
    public:
        const int w = 150;
        const int h = 150;
        GateNode(Vector2 pos, N *simNode);
        void updatePinPosition() override = 0;
        void render(int lod) const override = 0;
        N *simNode;
    };
}


#endif //BUILDIT_GATENODE_H
