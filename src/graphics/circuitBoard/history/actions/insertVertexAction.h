//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTVERTEXACTION_H
#define BUILDIT_INSERTVERTEXACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"

class InsertVertexAction : public WiresAction {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertVertexAction(const std::shared_ptr<Vertex>& vertex, Wires* wires, WiresRenderer* renderer, bool reversed)
        : vertex(vertex), WiresAction(wires, renderer, reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_INSERTVERTEXACTION_H
