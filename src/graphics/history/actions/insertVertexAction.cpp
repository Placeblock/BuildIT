//
// Created by felix on 8/3/24.
//

#include "insertVertexAction.h"

void InsertVertexAction::execute(bool lastInBatch) {
    if (this->splitWire == nullptr) {
        this->splitWire = wires->getWire(this->vertex->cell);
        if (this->splitWire == nullptr) {
            throw std::logic_error("Tried to insert vertex above no wire");
        }
    }
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = std::make_shared<Wire>(this->splitWire->start, this->vertex, this->splitWire->network, this->splitWire->color);
        this->createdWires[1] = std::make_shared<Wire>(this->vertex, this->splitWire->end, this->splitWire->network, this->splitWire->color);
    }

    this->vertex->network = this->splitWire->network;
    //TODO: CHECK IF VERTEX HAS ALREADY CONNECTED WIRES AND OVERWRITE NETWORK
    wires->deleteWire(this->splitWire);
    wires->addVertex(this->vertex);
    wires->addWire(this->createdWires[0]);
    wires->addWire(this->createdWires[1]);

    this->createdWires[0]->network->connect(this->createdWires[0]);
    this->createdWires[1]->network->connect(this->createdWires[1]);

    if (lastInBatch) this->regenerate();
}

void InsertVertexAction::rewind(bool lastInBatch) {
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = *this->vertex->wires.begin();
        this->createdWires[1] = *(++this->vertex->wires.begin());
    }
    if (this->splitWire == nullptr) {
        this->splitWire = std::make_shared<Wire>(
        this->createdWires[0]->getOther(this->vertex),
        this->createdWires[1]->getOther(this->vertex),
        this->createdWires[0]->color);
        this->splitWire->network = this->vertex->network;
    }
    wires->deleteWire(this->createdWires[0]);
    wires->deleteWire(this->createdWires[1]);
    wires->deleteVertex(this->vertex);
    wires->addWire(this->splitWire);
    this->splitWire->network->connect(this->splitWire);

    if (lastInBatch) this->regenerate();
}
