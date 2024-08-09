//
// Created by felix on 8/2/24.
//

#include "createVertexAction.h"

void CreateVertexAction::execute(bool lastInBatch) {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>(this->vertex->color);
        this->vertex->network = this->createdNetwork;
    }
    wires->networks.insert(this->createdNetwork);
    wires->addVertex(this->vertex);

    if (lastInBatch) this->regenerate();
}

void CreateVertexAction::rewind(bool lastInBatch) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->vertex->network;
    }
    wires->deleteVertex(this->vertex);
    wires->networks.erase(this->createdNetwork);

    if (lastInBatch) this->regenerate();
}

CreateVertexAction::CreateVertexAction(const std::shared_ptr<Vertex>& vertex, Wires *wires, WiresRenderer *renderer,
                                       bool reversed)
        : vertex(vertex), WiresAction(wires, renderer, reversed) {
    this->createdNetwork = vertex->network;
}