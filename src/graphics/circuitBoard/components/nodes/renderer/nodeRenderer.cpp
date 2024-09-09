//
// Created by felix on 9/7/24.
//

#include "nodeRenderer.h"

void NodeRenderer::addNode(Node *node) {
    node->Movable::subscribe(this);
}

void NodeRenderer::removeNode(Node *node) {
    node->Movable::unsubscribe(this);
}
