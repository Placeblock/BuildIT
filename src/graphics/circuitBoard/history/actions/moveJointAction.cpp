//
// Created by felix on 8/3/24.
//

#include "moveJointAction.h"

void MoveJointAction::execute(bool lastInBatch) {
    this->oldCell = this->joint->cell;

    this->wires->moveJoint(this->joint.get(), this->newCell);

    this->updateCellData(wires, renderer);
}

void MoveJointAction::rewind(bool lastInBatch) {
    this->wires->moveJoint(this->joint.get(), this->oldCell);

    this->updateCellData(wires, renderer);
}

void MoveJointAction::updateCellData(Wires *wires, WiresRenderer *renderer) {
    renderer->updateVertexPos(wires->getJointIndex(this->joint.get()), this->joint->cell);
    for (const auto &wire: this->joint->wires) {
        renderer->updateWirePos(wires->getWireIndex(wire), wire->start->cell, wire->end->cell);
    }
}
