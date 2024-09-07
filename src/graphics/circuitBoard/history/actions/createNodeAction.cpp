//
// Created by felix on 8/22/24.
//

#include "createNodeAction.h"

void CreateNodeAction::execute(bool lastInBatch) {
    this->nodeContainer->addNode(this->node);
    this->renderer->addNode(this->node.get());
}

void CreateNodeAction::rewind(bool lastInBatch) {
    this->nodeContainer->removeNode(this->node.get());
    this->renderer->removeNode(this->node.get());
}
