//
// Created by felix on 8/3/24.
//

#include "moveVertexAction.h"

void MoveVertexAction::execute(bool lastInBatch) {
    this->oldCell = this->vertex->pos;
    this->vertex->pos = this->newCell;
    if (wires->cellMap[this->oldCell] == this->vertex.get()) { // When moving multiple this could be false
        wires->cellMap.erase(this->oldCell);
    }
    wires->cellMap[this->newCell] = this->vertex.get();

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::rewind(bool lastInBatch) {
    this->vertex->pos = this->oldCell;
    if (wires->cellMap[this->newCell] == this->vertex.get()) { // When moving multiple this could be false
        wires->cellMap.erase(this->newCell);
    }
    wires->cellMap[this->oldCell] = this->vertex.get();

    this->updateCellData(wires, renderer);
}

void MoveVertexAction::updateCellData(Wires *wires, WiresRenderer *renderer) {
    renderer->updateVertexPos(wires->getVertexIndex(this->vertex.get()), this->vertex->pos);
    for (const auto &wire: this->vertex->wires) {
        renderer->updateWirePos(wires->getWireIndex(wire), wire->start->pos, wire->end->pos);
    }
}
