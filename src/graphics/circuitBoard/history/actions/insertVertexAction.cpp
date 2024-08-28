//
// Created by felix on 8/3/24.
//

#include "insertVertexAction.h"

void InsertVertexAction::execute(bool lastInBatch) {
    if (this->splitWire == nullptr) {
        this->splitWire = wires->getOwningRef(wires->getWire(this->vertex->cell));
        if (this->splitWire == nullptr) {
            throw std::logic_error("Tried to insert vertex above no wire");
        }
    }
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = std::make_shared<Wire>(this->splitWire->start, this->vertex.get(), this->splitWire->network, this->splitWire->color);
        this->createdWires[1] = std::make_shared<Wire>(this->vertex.get(), this->splitWire->end, this->splitWire->network, this->splitWire->color);
    }

    this->vertex->network = this->splitWire->network;
    //TODO: CHECK IF VERTEX HAS ALREADY CONNECTED WIRES AND OVERWRITE NETWORK
    wires->removeWire(this->splitWire.get());
    wires->addJoint(this->vertex);
    wires->addWire(this->createdWires[0]);
    wires->addWire(this->createdWires[1]);

    this->createdWires[0]->network->connect(this->createdWires[0].get());
    this->createdWires[1]->network->connect(this->createdWires[1].get());

    if (lastInBatch) this->regenerate();
}

void InsertVertexAction::rewind(bool lastInBatch) {
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = this->wires->getOwningRef(*this->vertex->wires.begin());
        this->createdWires[1] = this->wires->getOwningRef(*(++this->vertex->wires.begin()));
    }
    if (this->splitWire == nullptr) {
        this->splitWire = std::make_shared<Wire>(
        this->createdWires[0]->getOther(this->vertex.get()),
        this->createdWires[1]->getOther(this->vertex.get()),
        this->createdWires[0]->color);
        this->splitWire->network = this->vertex->network;
    }
    wires->removeWire(this->createdWires[0].get());
    wires->removeWire(this->createdWires[1].get());
    wires->removeJoint(this->vertex.get());
    wires->addWire(this->splitWire);
    this->splitWire->network->connect(this->splitWire.get());

    if (lastInBatch) this->regenerate();
}
