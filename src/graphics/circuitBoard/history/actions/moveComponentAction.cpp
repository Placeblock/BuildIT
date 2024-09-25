//
// Created by felix on 8/3/24.
//

#include "moveComponentAction.h"

void MoveComponentAction::execute(bool lastInBatch) {
    this->movable->move(this->delta);
}

void MoveComponentAction::rewind(bool lastInBatch) {
    this->movable->move(-this->delta);
}
