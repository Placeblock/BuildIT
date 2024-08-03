//
// Created by felix on 8/2/24.
//

#include "createVertexAction.h"

void CreateVertexAction::Execute(Wires* wires) {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>(this->vertex->color);
        this->vertex->network = this->createdNetwork;
    }
    wires->networks.insert(this->createdNetwork);
    wires->addVertex(this->vertex);
}

void CreateVertexAction::Rewind(Wires* wires) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->vertex->network;
    }
    wires->deleteVertex(this->vertex);
    wires->networks.erase(this->createdNetwork);
}

CreateVertexAction::CreateVertexAction(std::shared_ptr<Vertex> vertex) {
    this->vertex = vertex;
}
