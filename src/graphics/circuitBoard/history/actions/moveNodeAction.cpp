//
// Created by felix on 8/4/24.
//

#include "moveNodeAction.h"

void MoveNodeAction::execute(bool lastInBatch) {
    this->oldPos = this->node->pos;
    this->nodes->updatePos(this->node, this->newPos, true);
}

void MoveNodeAction::rewind(bool lastInBatch) {
    this->newPos = this->node->pos;
    this->nodes->updatePos(this->node, this->oldPos, true);
}
