//
// Created by felix on 8/2/24.
//

#include "createVertexAction.h"

void CreateVertexAction::execute(bool lastInBatch) {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>();
        this->vertex->network = this->createdNetwork.get();
    }
    this->wires->networks.insert(this->createdNetwork);
    this->wires->addVertex(this->vertex);

    if (lastInBatch) this->regenerate();
}

void CreateVertexAction::rewind(bool lastInBatch) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->wires->getOwningRef(this->vertex->network);
    }
    this->wires->deleteVertex(this->vertex.get());
    this->wires->networks.erase(this->createdNetwork);

    if (lastInBatch) this->regenerate();
}

CreateVertexAction::CreateVertexAction(const std::shared_ptr<Vertex>& vertex, Wires *wires, WiresRenderer *renderer,
                                       bool reversed)
        : vertex(vertex), WiresAction(wires, renderer, reversed) {
}