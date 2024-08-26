//
// Created by felix on 8/3/24.
//

#include "moveVertexAction.h"

void MoveVertexAction::execute(bool lastInBatch) {
    this->oldCell = this->vertex->cell;
    this->vertex->cell = this->newCell;

    if (wires->cellMap[this->oldCell] == this->vertex.get()) { // When moving multiple this could be false
        wires->cellMap.erase(this->oldCell);
    }
    wires->cellMap[this->newCell] = this->vertex.get();

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::rewind(bool lastInBatch) {
    this->vertex->cell = this->oldCell;
    if (wires->cellMap[this->newCell] == this->vertex.get()) { // When moving multiple this could be false
        wires->cellMap.erase(this->newCell);
    }
    wires->cellMap[this->oldCell] = this->vertex.get();

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::updateCellData(Wires *wires, WiresRenderer *renderer) {
    renderer->updateVertexPos(wires->getVertexIndex(this->vertex.get()), this->vertex->cell);
    for (const auto &wire: this->vertex->wires) {
        renderer->updateWirePos(wires->getWireIndex(wire), wire->start->cell, wire->end->cell);
    }
}
