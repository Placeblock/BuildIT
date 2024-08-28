//
// Created by felix on 8/3/24.
//

#include "insertJointAction.h"

void InsertJointAction::execute(bool lastInBatch) {
    if (this->splitWire == nullptr) {
        this->splitWire = wires->getOwningRef(wires->getWire(this->joint->cell));
        if (this->splitWire == nullptr) {
            throw std::logic_error("Tried to insert joint above no wire");
        }
    }
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = std::make_shared<Wire>(this->splitWire->start, this->joint.get(), this->splitWire->network, this->splitWire->color);
        this->createdWires[1] = std::make_shared<Wire>(this->joint.get(), this->splitWire->end, this->splitWire->network, this->splitWire->color);
    }

    this->joint->network = this->splitWire->network;
    //TODO: CHECK IF VERTEX HAS ALREADY CONNECTED WIRES AND OVERWRITE NETWORK
    wires->removeWire(this->splitWire.get());
    wires->addJoint(this->joint);
    wires->addWire(this->createdWires[0]);
    wires->addWire(this->createdWires[1]);

    this->createdWires[0]->network->connect(this->createdWires[0].get());
    this->createdWires[1]->network->connect(this->createdWires[1].get());

    if (lastInBatch) this->regenerate();
}

void InsertJointAction::rewind(bool lastInBatch) {
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = this->wires->getOwningRef(*this->joint->wires.begin());
        this->createdWires[1] = this->wires->getOwningRef(*(++this->joint->wires.begin()));
    }
    if (this->splitWire == nullptr) {
        this->splitWire = std::make_shared<Wire>(
        this->createdWires[0]->getOther(this->joint.get()),
        this->createdWires[1]->getOther(this->joint.get()),
        this->createdWires[0]->color);
        this->splitWire->network = this->joint->network;
    }
    wires->removeWire(this->createdWires[0].get());
    wires->removeWire(this->createdWires[1].get());
    wires->removeJoint(this->joint.get());
    wires->addWire(this->splitWire);
    this->splitWire->network->connect(this->splitWire.get());

    if (lastInBatch) this->regenerate();
}
