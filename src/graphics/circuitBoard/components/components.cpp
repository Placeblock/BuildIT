//
// Created by felix on 8/27/24.
//

#include "components.h"

void Components::moveNode(Node *node, glm::vec2 newPos, bool updateBuffer) {
}

void Components::addNode(const std::shared_ptr<Node> &node) {
    this->nodes.addNode(node);
}
