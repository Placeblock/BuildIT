//
// Created by felix on 8/3/24.
//

#include "moveJointAction.h"

void MoveJointAction::execute(bool lastInBatch) {
    this->oldCell = this->joint->cell;

    this->jointContainer->moveJoint(this->joint.get(), this->newCell);

    this->updateCellData();
}

void MoveJointAction::rewind(bool lastInBatch) {
    this->jointContainer->moveJoint(this->joint.get(), this->oldCell);

    this->updateCellData();
}

void MoveJointAction::updateCellData() {
    this->wiresRenderer->moveJoint(this->jointContainer->getJointIndex(this->joint.get()), this->joint->cell);
    for (const auto &wire: this->joint->wires) {
        this->wiresRenderer->moveWire(this->wireContainer->getWireIndex(wire), wire->start->cell, wire->end->cell);
    }
}
