//
// Created by felix on 8/28/24.
//

#include "nodes.h"

#include "graphics/circuitBoard/components/nodes/node.h"


bool NodeInteractionManager::isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored) {
    for (const auto &[_, node]: this->nodeMap) {
        if (node->intersects(pos)) return true;
    }
    return false;
}

void NodeInteractionManager::notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent &data) {
    Node *node = static_cast<Node*>(data.component);

    this->nodeMap[node->getPos()] = node;
    node->Movable::subscribe(this);
}

void NodeInteractionManager::notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent &data) {
    Node *node = static_cast<Node*>(data.component);
    node->Movable::unsubscribe(this);
    this->nodeMap.erase(node->getPos());
}

Node *NodeInteractionManager::getIntersectedNode(glm::vec2 pos) {
    for (const auto &[_, node]: this->nodeMap) {
        if (node->intersects(pos)) return node;
    }
    return nullptr;
}

void NodeInteractionManager::notify(Node *node, const MoveEvent &event) {
    /*
     * We have to check if the pos in the map really maps this node. Usually this should always be true, because
     * everything else would be a sign of desync, however when moving multiple joints at ones it can happen that one
     * overwrites the position of the other temporarely.
     */
    if (this->nodeMap.contains(node->getPos()) && this->nodeMap[node->getPos()] == node) {
        this->nodeMap[event.newPos] = this->nodeMap[node->getPos()];
        this->nodeMap.erase(node->getPos());
    }
}