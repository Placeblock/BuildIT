//
// Created by felix on 8/2/24.
//

#include "createVertexAction.h"

void CreateVertexAction::execute() {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>(this->vertex->color);
        this->vertex->network = this->createdNetwork;
    }
    wires->networks.insert(this->createdNetwork);
    wires->addVertex(this->vertex);

    this->checkRegenerate();
}

void CreateVertexAction::rewind() {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->vertex->network;
    }
    wires->deleteVertex(this->vertex);
    wires->networks.erase(this->createdNetwork);

    this->checkRegenerate();
}

CreateVertexAction::CreateVertexAction(std::shared_ptr<Vertex> vertex, Wires *wires, WiresRenderer *renderer,
                                       bool regenerate, bool reversed)
        : vertex(std::move(vertex)), WiresAction(wires, renderer, regenerate, reversed) {
    this->createdNetwork = vertex->network;
}