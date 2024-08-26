//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEVERTEXACTION_H
#define BUILDIT_CREATEVERTEXACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"
#include "graphics/circuitBoard/elements/nodes/node.h"

/**
 * Creates a single vertex with a new network
 */
class CreateVertexAction : public WiresAction {
private:
    Sim::Simulation* simulation;
    Nodes* nodes;
    std::shared_ptr<Vertex> vertex;
    std::shared_ptr<Network> createdNetwork;
public:
    CreateVertexAction(Sim::Simulation *simulation, Nodes *nodes, const std::shared_ptr<Vertex> &vertex, Wires *wires,
                       WiresRenderer *renderer, bool reversed);
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEVERTEXACTION_H
