//
// Created by felix on 8/28/24.
//

#include "nodes.h"


bool Nodes::isOccupied(glm::vec2 pos, std::unordered_set<Node*> ignored) {
    return std::any_of(this->nodes.begin(), this->nodes.end(), [&pos, &ignored](const std::shared_ptr<Node>& node){
        if (ignored.contains(node.get())) return false;
        return node->intersects(pos);
    });
}

void Nodes::addNode(Node* node) {
    this->nodeMap[node->getPos()] = node;
    node->Movable::subscribe(this);
}

void Nodes::removeNode(Node* node) {
    const auto iter = std::find_if(this->nodes.begin(), this->nodes.end(), [&node](const std::shared_ptr<Node>& n){
        return n.get() == node;
    });
    assert(iter != this->nodes.end() && "Tried to remove non existent node from nodes");
    this->nodeMap.erase(node->getPos());
}

Node *Nodes::getIntersectedNode(glm::vec2 pos) {
    auto iterator = std::find_if(this->nodes.begin(), this->nodes.end(), [&pos](const std::shared_ptr<Node>& node){
        return node->intersects(pos);
    });
    if (iterator == this->nodes.end()) return nullptr;
    return iterator->get();
}

void Nodes::update(Node *node, const MoveEvent& event) {
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

Nodes::~Nodes() {
    for (const auto &item: this->nodes) {
        item->Movable::unsubscribe(this);
    }
}
