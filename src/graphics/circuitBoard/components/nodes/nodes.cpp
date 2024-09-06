//
// Created by felix on 8/28/24.
//

#include "nodes.h"


bool Nodes::isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored) {
    for (const auto &[_, node]: this->nodeMap) {
        if (node->intersects(pos)) return true;
    }
    return false;
}

void Nodes::addNode(Node* node) {
    this->nodeMap[node->getPos()] = node;
    node->Movable::subscribe(this->addSubject(node));
}

void Nodes::removeNode(Node* node) {
    node->Movable::unsubscribe(this->removeSubject(node));
    this->nodeMap.erase(node->getPos());
}

Node *Nodes::getIntersectedNode(glm::vec2 pos) {
    for (const auto &[_, node]: this->nodeMap) {
        if (node->intersects(pos)) return node;
    }
    return nullptr;
}

Nodes::~Nodes() {
    for (const auto &[pos, node]: this->nodeMap) {
        node->Movable::unsubscribe(this->removeSubject(node));
    }
}

void Nodes::update(const MoveEvent &event, Node *node) {
    /*
     * We have to check if the cell in the map really maps this node. Usually this should always be true, because
     * everything else would be a sign of desync, however when moving multiple nodes at ones it can happen that one
     * overwrites the position of the other temporarely.
     */
    if (this->nodeMap.contains(node->getPos()) && this->nodeMap[node->getPos()] == node) {
        this->nodeMap[event.newPos] = this->nodeMap[node->getPos()];
        this->nodeMap.erase(node->getPos());
    }
}
