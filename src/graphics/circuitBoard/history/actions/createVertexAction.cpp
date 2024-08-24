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

    if (this->nodes->inputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->inputPins[this->vertex->cell];
        node->onInputConnect(node->getInputPinIndex(this->vertex->cell), this->vertex.get());
    }
    if (this->nodes->outputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->outputPins[this->vertex->cell];
        node->onOutputConnect(node->getOutputPinIndex(this->vertex->cell), this->vertex.get());
    }

    if (lastInBatch) this->regenerate();
}

void CreateVertexAction::rewind(bool lastInBatch) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->wires->getOwningRef(this->vertex->network);
    }
    this->wires->deleteVertex(this->vertex.get());
    this->wires->networks.erase(this->createdNetwork);

    if (this->nodes->inputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->inputPins[this->vertex->cell];
        node->onInputDisconnect(node->getInputPinIndex(this->vertex->cell), this->vertex.get());
    }
    if (this->nodes->outputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->outputPins[this->vertex->cell];
        node->onOutputDisconnect(node->getOutputPinIndex(this->vertex->cell), this->vertex.get());
    }

    if (lastInBatch) this->regenerate();
}

CreateVertexAction::CreateVertexAction(const std::shared_ptr<Vertex>& vertex, Wires *wires, WiresRenderer *renderer,
                                       Nodes* nodes, bool reversed)
        : vertex(vertex), nodes(nodes), WiresAction(wires, renderer, reversed) {
}