//
// Created by felix on 8/2/24.
//

#include "createJointAction.h"

void CreateJointAction::execute(bool lastInBatch) {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>();
        this->vertex->network = this->createdNetwork.get();
    }
    this->wires->networks.insert(this->createdNetwork);
    this->wires->addJoint(this->vertex);

    if (this->nodes->inputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->inputPins[this->vertex->cell];
        this->vertex->network->onChildConnect(this->simulation, this->vertex.get(), node, node->getInputPinIndex(this->vertex->cell));
    }
    if (this->nodes->outputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->outputPins[this->vertex->cell];
        this->vertex->network->onParentConnect(this->simulation, this->vertex.get(), node, node->getOutputPinIndex(this->vertex->cell));
    }

    if (lastInBatch) this->regenerate();
}

void CreateJointAction::rewind(bool lastInBatch) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->wires->getOwningRef(this->vertex->network);
    }
    this->wires->deleteJoint(this->vertex.get());
    this->wires->networks.erase(this->createdNetwork);

    if (this->nodes->inputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->inputPins[this->vertex->cell];
        this->vertex->network->onChildDisconnect(this->simulation, this->vertex.get(), node, node->getInputPinIndex(this->vertex->cell));
    }
    if (this->nodes->outputPins.contains(this->vertex->cell)) {
        Node* node = this->nodes->outputPins[this->vertex->cell];
        this->vertex->network->onParentDisconnect(this->simulation, node, node->getOutputPinIndex(this->vertex->cell));
    }

    if (lastInBatch) this->regenerate();
}

CreateJointAction::CreateJointAction(Sim::Simulation *simulation, Nodes *nodes, const std::shared_ptr<Vertex> &joint,
                                     Wires *wires,
                                     WiresRenderer *renderer, bool reversed)
        : simulation(simulation), vertex(joint), nodes(nodes), WiresAction(wires, renderer, reversed) {
}