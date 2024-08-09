//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEVERTEXACTION_H
#define BUILDIT_CREATEVERTEXACTION_H


#include "graphics/history/wiresAction.h"

/**
 * Creates a single vertex with a new network
 */
class CreateVertexAction : public WiresAction {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Network> createdNetwork;
public:
    CreateVertexAction(std::shared_ptr<Vertex> vertex, Wires* wires, WiresRenderer* renderer, bool regenerate, bool reversed);
    void execute() override;
    void rewind() override;
};


#endif //BUILDIT_CREATEVERTEXACTION_H
