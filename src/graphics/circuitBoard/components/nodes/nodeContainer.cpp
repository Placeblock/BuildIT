//
// Created by felix on 9/7/24.
//
#include "nodeContainer.h"

void NodeContainer::addNode(const std::shared_ptr<Node>& node) {
    this->nodes.push_back(node);
    this->Subject<ComponentAddEvent>::notify({node.get()});
}

void NodeContainer::removeNode(Node *node) {
    const auto iter = std::find_if(this->nodes.begin(), this->nodes.end(), [&node](const std::shared_ptr<Node>& n){
        return n.get() == node;
    });
    assert(iter != this->nodes.end() && "Tried to remove non existent node from nodes");
    this->nodes.erase(iter);
    this->Subject<ComponentRemoveEvent>::notify({node});
}

const std::list<std::shared_ptr<Node>> *NodeContainer::getNodes() const {
    return &this->nodes;
}


