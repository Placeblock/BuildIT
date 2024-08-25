//
// Created by felix on 8/22/24.
//

#include "createNodeAction.h"

void CreateNodeAction::execute(bool lastInBatch) {
    this->nodes->addNode(this->node);
    this->node->renderer->addNode(this->node.get());
    this->node->simulation->addNode(this->node->simNode);
}

void CreateNodeAction::rewind(bool lastInBatch) {
    this->nodes->removeNode(this->node.get());
    this->node->renderer->removeNode(this->node.get());
    this->node->simulation->removeNode(this->node->simNode);
}
