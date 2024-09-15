//
// Created by felix on 8/3/24.
//

#include "moveComponentAction.h"

void MoveComponentAction::execute(bool lastInBatch) {
    this->oldPos = this->component->getPos();

    this->component->move(this->newPos);
}

void MoveComponentAction::rewind(bool lastInBatch) {
    this->newPos = this->component->getPos();

    this->component->move(this->oldPos);
}
