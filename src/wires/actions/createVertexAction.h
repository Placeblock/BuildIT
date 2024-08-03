//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEVERTEXACTION_H
#define BUILDIT_CREATEVERTEXACTION_H


#include "wires/action.h"

/**
 * Creates a single vertex with a new network
 */
class CreateVertexAction : public Action {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Network> createdNetwork;
public:
    explicit CreateVertexAction(std::shared_ptr<Vertex> vertex);
    void Execute(Wires* wires) override;
    void Rewind(Wires* wires) override;
};


#endif //BUILDIT_CREATEVERTEXACTION_H
