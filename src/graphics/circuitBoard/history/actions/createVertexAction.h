//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEVERTEXACTION_H
#define BUILDIT_CREATEVERTEXACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"

/**
 * Creates a single vertex with a new network
 */
class CreateVertexAction : public WiresAction {
private:
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Network> createdNetwork;
public:
    CreateVertexAction(const std::shared_ptr<Vertex>& vertex, Wires* wires, WiresRenderer* renderer, bool reversed);
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEVERTEXACTION_H
