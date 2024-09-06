//
// Created by felix on 9/7/24.
//
#include "nodeContainer.h"

void NodeContainer::addNode(const std::shared_ptr<Node>& node) {
    this->nodes.push_back(node);
    this->Subject<NodeAddEvent>::notify({node.get()});
}

void NodeContainer::removeNode(Node *node) {
    const auto iter = std::find_if(this->nodes.begin(), this->nodes.end(), [&node](const std::shared_ptr<Node>& n){
        return n.get() == node;
    });
    assert(iter != this->nodes.end() && "Tried to remove non existent node from nodes");
    this->nodes.erase(iter);
    this->Subject<NodeRemoveEvent>::notify({node});
}

