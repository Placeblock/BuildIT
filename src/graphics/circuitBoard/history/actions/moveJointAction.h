//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEJOINTACTION_H
#define BUILDIT_MOVEJOINTACTION_H


#include <utility>

#include "graphics/circuitBoard/history/wiresAction.h"
#include "simulation/simulation.h"
#include "graphics/circuitBoard/components/nodes/node.h"

class MoveJointAction : public WiresAction {
private:
    Sim::Simulation* simulation;
    Nodes* nodes;
    std::shared_ptr<Joint> joint;
    glm::vec2 newCell;
    glm::vec2 oldCell;
    void updateCellData(Wires* wires, WiresRenderer* renderer);
public:
    MoveJointAction(Sim::Simulation *simulation, Nodes *nodes, const std::shared_ptr<Joint>& joint, glm::vec2 newCell, Wires* wires, WiresRenderer* renderer)
        : simulation(simulation), nodes(nodes), joint(joint), newCell(newCell), WiresAction(wires, renderer, false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVEJOINTACTION_H
