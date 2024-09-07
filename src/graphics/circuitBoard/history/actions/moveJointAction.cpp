//
// Created by felix on 8/3/24.
//

#include "moveJointAction.h"

void MoveJointAction::execute(bool lastInBatch) {
    this->oldPos = this->joint->getPos();

    this->joint->move(this->newPos);
}

void MoveJointAction::rewind(bool lastInBatch) {
    this->joint->move(this->oldPos);
}
