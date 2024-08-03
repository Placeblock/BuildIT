//
// Created by felix on 8/3/24.
//

#include "moveVertexAction.h"

MoveVertexAction::MoveVertexAction(std::shared_ptr<Vertex> vertex, glm::vec2 newCell) {
    this->vertex = vertex;
    this->newCell = newCell;
}

void MoveVertexAction::Execute(Wires *cables) {
    this->oldCell = this->vertex->cell;
    this->vertex->cell = this->newCell;
    cables->cellMap.erase(this->oldCell);
    cables->cellMap[this->newCell] = this->vertex;
}

void MoveVertexAction::Rewind(Wires *cables) {
    this->vertex->cell = this->oldCell;
    cables->cellMap.erase(this->newCell);
    cables->cellMap[this->oldCell] = this->vertex;
}
