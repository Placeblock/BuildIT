//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEVERTEXACTION_H
#define BUILDIT_MOVEVERTEXACTION_H


#include <utility>

#include "graphics/circuitBoard/history/wiresAction.h"

class MoveVertexAction : public WiresAction {
private:
    Sim::Simulation* simulation;
    Nodes* nodes;
    std::shared_ptr<Vertex> vertex;
    glm::vec2 newCell;
    glm::vec2 oldCell;
    void updateCellData(Wires* wires, WiresRenderer* renderer);
public:
    MoveVertexAction(Sim::Simulation *simulation, Nodes *nodes, const std::shared_ptr<Vertex>& vertex, glm::vec2 newCell, Wires* wires, WiresRenderer* renderer)
        : simulation(simulation), nodes(nodes), vertex(vertex), newCell(newCell), WiresAction(wires, renderer, false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVEVERTEXACTION_H
