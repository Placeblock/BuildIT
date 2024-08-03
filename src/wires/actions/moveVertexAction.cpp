//
// Created by felix on 8/3/24.
//

#include "moveVertexAction.h"

MoveVertexAction::MoveVertexAction(std::shared_ptr<Vertex> vertex, glm::vec2 newCell) {
    this->vertex = vertex;
    this->newCell = newCell;
}

void MoveVertexAction::Execute(Wires *wires, WiresRenderer* renderer, bool regenerate) {
    this->oldCell = this->vertex->cell;
    this->vertex->cell = this->newCell;
    wires->cellMap.erase(this->oldCell);
    wires->cellMap[this->newCell] = this->vertex;

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::Rewind(Wires *wires, WiresRenderer* renderer, bool regenerate) {
    this->vertex->cell = this->oldCell;
    wires->cellMap.erase(this->newCell);
    wires->cellMap[this->oldCell] = this->vertex;

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::updateCellData(Wires *wires, WiresRenderer *renderer) {
    renderer->updateVertexPos(wires->getVertexIndex(this->vertex), this->newCell);
    for (const auto &wire: this->vertex->wires) {
        renderer->updateWirePos(wires->getWireIndex(wire), wire->start->cell, wire->end->cell);
    }
}
