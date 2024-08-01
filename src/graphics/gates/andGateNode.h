//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_ANDGATENODE_H
#define BUILDIT_ANDGATENODE_H


#include "gate.h"
#include "gateNode.h"

namespace Graphics {
    class AndGateNode : public GateNode<Sim::AndGate> {
    public:
        AndGateNode(Vector2 pos, Sim::AndGate *simNode);
        void render(int lod) const override;
    };
}


#endif //BUILDIT_ANDGATENODE_H
