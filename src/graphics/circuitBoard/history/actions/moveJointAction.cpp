//
// Created by felix on 8/3/24.
//

#include "moveJointAction.h"

void MoveJointAction::execute(bool lastInBatch) {
    this->oldCell = this->joint->pos;

    this->jointContainer->moveJoint(this->joint.get(), this->newCell);

    this->updateCellData();
}

void MoveJointAction::rewind(bool lastInBatch) {
    this->jointContainer->moveJoint(this->joint.get(), this->oldCell);

    this->updateCellData();
}

void MoveJointAction::updateCellData() {
    this->wiresRenderer->moveJoint(this->jointContainer->getJointIndex(this->joint.get()), this->joint->pos);
    for (const auto &wire: this->joint->wires) {
        this->wiresRenderer->moveWire(this->wireContainer->getWireIndex(wire), wire->start->pos, wire->end->pos);
    }
}
