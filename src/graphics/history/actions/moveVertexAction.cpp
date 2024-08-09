//
// Created by felix on 8/3/24.
//

#include "moveVertexAction.h"

void MoveVertexAction::execute() {
    this->oldCell = this->vertex->cell;
    this->vertex->cell = this->newCell;
    if (wires->cellMap[this->oldCell] == this->vertex) { // When moving multiple this could be false
        wires->cellMap.erase(this->oldCell);
    }
    wires->cellMap[this->newCell] = this->vertex;

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::rewind() {
    this->vertex->cell = this->oldCell;
    if (wires->cellMap[this->newCell] == this->vertex) { // When moving multiple this could be false
        wires->cellMap.erase(this->newCell);
    }
    wires->cellMap[this->oldCell] = this->vertex;

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::updateCellData(Wires *wires, WiresRenderer *renderer) {
    renderer->updateVertexPos(wires->getVertexIndex(this->vertex), this->newCell);
    for (const auto &wire: this->vertex->wires) {
        renderer->updateWirePos(wires->getWireIndex(wire), wire->start->cell, wire->end->cell);
    }
}
