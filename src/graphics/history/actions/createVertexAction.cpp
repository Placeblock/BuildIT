//
// Created by felix on 8/2/24.
//

#include "createVertexAction.h"

void CreateVertexAction::Execute(Wires* wires, WiresRenderer* renderer, bool regenerate) {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>(this->vertex->color);
        this->vertex->network = this->createdNetwork;
    }
    wires->networks.insert(this->createdNetwork);
    wires->addVertex(this->vertex);

    this->checkRegenerate(wires, renderer, regenerate);
}

void CreateVertexAction::Rewind(Wires* wires, WiresRenderer* renderer, bool regenerate) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->vertex->network;
    }
    wires->deleteVertex(this->vertex);
    wires->networks.erase(this->createdNetwork);

    this->checkRegenerate(wires, renderer, regenerate);
}

CreateVertexAction::CreateVertexAction(const std::shared_ptr<Vertex>& vertex) : vertex(vertex) {
    this->createdNetwork = vertex->network;
}
