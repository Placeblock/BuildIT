//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_NOTGATENODE_H
#define BUILDIT_NOTGATENODE_H


#include "gate.h"
#include "gateNode.h"

namespace Graphics {
    class NotGateNode : public GateNode<Sim::NotGate> {
    public:
        NotGateNode(Vector2 pos, Sim::NotGate *simNode);
        void render(int lod) const override;
    };
}

#endif //BUILDIT_NOTGATENODE_H
