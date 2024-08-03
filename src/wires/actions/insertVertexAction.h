//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTVERTEXACTION_H
#define BUILDIT_INSERTVERTEXACTION_H


#include "wires/action.h"

class InsertVertexAction : public Action {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    explicit InsertVertexAction(std::shared_ptr<Vertex> vertex);
    void Execute(Wires* wires) override;
    void Rewind(Wires* wires) override;
};


#endif //BUILDIT_INSERTVERTEXACTION_H
