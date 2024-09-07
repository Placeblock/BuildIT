//
// Created by felix on 8/28/24.
//

#include "nodes.h"


NodeInteractionManager::NodeInteractionManager(Subject<NodeAddEvent> *nodeAddSubject,
                                               Subject<NodeAddEvent> *nodeRemoveSubject)
                                               : nodeAddSubject(nodeAddSubject), nodeRemoveSubject(nodeRemoveSubject){
    this->nodeAddSubject->subscribe(this);
    this->nodeRemoveSubject->subscribe(this);
}


bool NodeInteractionManager::isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored) {
    for (const auto &[_, node]: this->nodeMap) {
        if (node->intersects(pos)) return true;
    }
    return false;
}

void NodeInteractionManager::update(Subject<NodeAddEvent> *subject, const NodeAddEvent &data) {
    this->nodeMap[data.node->getPos()] = data.node;
    data.node->Movable<Node>::subscribe(this);
}

void NodeInteractionManager::update(Subject<NodeRemoveEvent> *subject, const NodeRemoveEvent &data) {
    data.node->Movable<Node>::unsubscribe(this);
    this->nodeMap.erase(data.node->getPos());
}

Node *NodeInteractionManager::getIntersectedNode(glm::vec2 pos) {
    for (const auto &[_, node]: this->nodeMap) {
        if (node->intersects(pos)) return node;
    }
    return nullptr;
}

void NodeInteractionManager::update(Subject<MoveEvent<Node>> *subject, const MoveEvent<Node> &event) {
    Node *node = static_cast<Node*>(subject);
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