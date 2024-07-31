//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_GATENODE_H
#define BUILDIT_GATENODE_H


#include "node.h"

namespace Graphics {
    class GateNode : public Node {
    public:
        GateNode(int x, int y) : Node(x, y) {};
        const int w = 20, h = 20;
        bool updateRedered(int lod, Vector2 offset) override;
        void render(int lod) const override;
    };
}


#endif //BUILDIT_GATENODE_H
