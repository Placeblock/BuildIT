//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEJOINTACTION_H
#define BUILDIT_CREATEJOINTACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"
#include "graphics/circuitBoard/components/nodes/node.h"

/**
 * Creates a single vertex with a new network
 */
class CreateJointAction : public WiresAction {
private:
    Sim::Simulation* simulation;
    Nodes* nodes;
    std::shared_ptr<Joint> vertex;
    std::shared_ptr<Network> createdNetwork;
public:
    CreateJointAction(Sim::Simulation *simulation, Nodes *nodes, const std::shared_ptr<Joint> &joint, Wires *wires,
                      WiresRenderer *renderer, bool reversed);
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEJOINTACTION_H
