//
// Created by felix on 8/4/24.
//

#include "moveNodeAction.h"

void MoveNodeAction::execute(bool lastInBatch) {
    this->oldCell = this->node->pos;
    this->node->renderer->moveNode(this->node.get(), this->newCell);
    this->nodeContainer->moveNode(this->node.get(), this->newCell, true);
}

void MoveNodeAction::rewind(bool lastInBatch) {
    this->newCell = this->node->pos;
    this->node->renderer->moveNode(this->node.get(), this->oldCell);
    this->nodeContainer->moveNode(this->node.get(), this->oldCell, true);
}
