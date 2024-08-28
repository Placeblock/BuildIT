//
// Created by felix on 8/27/24.
//

#include "components.h"

void Components::moveNode(Node *node, glm::vec2 newPos, bool updateBuffer) {
    this->nodes.moveNode(node, newPos, updateBuffer);
}

void Components::addNode(const std::shared_ptr<Node> &node) {
    this->nodes.addNode(node);
}

void Components::removeNode(Node *node) {
    this->nodes.removeNode(node);
}

void Components::addJoint(const std::shared_ptr<Joint> &joint) {
    this->wires.addJoint(joint);
}

void Components::removeJoint(Joint *joint) {
    this->wires.removeJoint(joint);
}

void Components::moveJoint(Joint *joint, glm::vec2 newPos) {
    this->wires.moveJoint(joint, newPos);
}
