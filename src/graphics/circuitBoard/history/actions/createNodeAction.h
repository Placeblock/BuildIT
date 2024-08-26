//
// Created by felix on 8/22/24.
//

#ifndef BUILDIT_CREATENODEACTION_H
#define BUILDIT_CREATENODEACTION_H


#include "graphics/circuitBoard/history/nodeAction.h"

class CreateNodeAction : public NodeAction {
private:
    Sim::Simulation* simulation;
    std::shared_ptr<Node> node;
public:
    explicit CreateNodeAction(Sim::Simulation* simulation, Nodes* nodes, bool reversed, const std::shared_ptr<Node>& node)
    : simulation(simulation), node(node), NodeAction(nodes, reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATENODEACTION_H
