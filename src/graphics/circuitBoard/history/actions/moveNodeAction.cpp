//
// Created by felix on 8/4/24.
//

#include "moveNodeAction.h"

void MoveNodeAction::execute(bool lastInBatch) {
    this->oldPos = this->node->getPos();
    this->node->move(this->newPos);
}

void MoveNodeAction::rewind(bool lastInBatch) {
    this->newPos = this->node->getPos();
    this->node->move(this->oldPos);
}
