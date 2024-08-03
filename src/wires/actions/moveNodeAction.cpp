//
// Created by felix on 8/3/24.
//

#include "moveNodeAction.h"

MoveNodeAction::MoveNodeAction(std::shared_ptr<Node> node, glm::vec2 newCell) {
    this->node = node;
    this->newCell = newCell;
}

void MoveNodeAction::Execute(Wires *cables) {
    this->oldCell = this->node->cell;
    this->node->cell = this->newCell;
}

void MoveNodeAction::Rewind(Wires *cables) {
    this->node->cell = this->oldCell;
}
