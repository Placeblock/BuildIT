//
// Created by felix on 8/3/24.
//

#include "insertJointAction.h"

void InsertJointAction::execute(bool lastInBatch) {
    if (this->splitWire == nullptr) {
        this->splitWire = this->wireContainer->getOwningRef(this->wireContainer->getWire(this->joint->pos));
        if (this->splitWire == nullptr) {
            throw std::logic_error("Tried to insert joint above no wire");
        }
    }
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = std::make_shared<Wire>(this->splitWire->start, this->joint.get(), this->splitWire->network);
        this->createdWires[1] = std::make_shared<Wire>(this->joint.get(), this->splitWire->end, this->splitWire->network);
    }

    this->joint->network = this->splitWire->network;
    //TODO: CHECK IF VERTEX HAS ALREADY CONNECTED WIRES AND OVERWRITE NETWORK (WHEN DRAGGING A JOINT OVER AN EXISTING WIRE)
    this->wireContainer->removeWire(this->splitWire.get());
    this->jointContainer->addJoint(this->joint);
    this->wireContainer->addWire(this->createdWires[0]);
    this->wireContainer->addWire(this->createdWires[1]);

    Network::connect(this->createdWires[0].get());
    Network::connect(this->createdWires[1].get());

    this->wiresRenderer->regenerateData(this->jointContainer, this->wireContainer);
}

void InsertJointAction::rewind(bool lastInBatch) {
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = this->wireContainer->getOwningRef(*this->joint->wires.begin());
        this->createdWires[1] = this->wireContainer->getOwningRef(*(++this->joint->wires.begin()));
    }
    if (this->splitWire == nullptr) {
        this->splitWire = std::make_shared<Wire>(
        this->createdWires[0]->getOther(this->joint.get()),
        this->createdWires[1]->getOther(this->joint.get()));
        this->splitWire->network = this->joint->network;
    }
    this->wireContainer->removeWire(this->createdWires[0].get());
    this->wireContainer->removeWire(this->createdWires[1].get());
    this->jointContainer->removeJoint(this->joint.get());
    this->wireContainer->addWire(this->splitWire);
    Network::connect(this->splitWire.get());

    this->wiresRenderer->regenerateData(this->jointContainer, this->wireContainer);
}
