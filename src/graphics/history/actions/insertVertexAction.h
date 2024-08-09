//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTVERTEXACTION_H
#define BUILDIT_INSERTVERTEXACTION_H


#include "graphics/history/wiresAction.h"

class InsertVertexAction : public WiresAction {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertVertexAction(std::shared_ptr<Vertex> vertex, Wires* wires, WiresRenderer* renderer, bool regenerate, bool reversed)
        : vertex(std::move(vertex)), WiresAction(wires, renderer, regenerate, reversed) {};
    void execute() override;
    void rewind() override;
};


#endif //BUILDIT_INSERTVERTEXACTION_H
