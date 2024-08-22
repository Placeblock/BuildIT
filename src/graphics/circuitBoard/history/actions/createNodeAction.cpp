//
// Created by felix on 8/22/24.
//

#include "createNodeAction.h"

void CreateNodeAction::execute(bool lastInBatch) {
    this->nodes->addNode(this->node);
    // ADD TO RENDERER
}

void CreateNodeAction::rewind(bool lastInBatch) {
    this->nodes->removeNode(this->node.get());
    // REMOVE FROM RENDERER
}
