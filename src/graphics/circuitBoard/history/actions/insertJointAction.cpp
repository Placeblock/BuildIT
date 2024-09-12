//
// Created by felix on 8/3/24.
//

#include "insertJointAction.h"

void InsertJointAction::execute(bool lastInBatch) {
    if (this->createdWires[0] == nullptr || this->createdWires[1] == nullptr) {
        this->createdWires[0] = std::make_shared<Wire>(this->splitWire->start, this->joint.get(), this->splitWire->getNetwork());
        this->createdWires[1] = std::make_shared<Wire>(this->joint.get(), this->splitWire->end, this->splitWire->getNetwork());
    }

    //TODO: CHECK IF VERTEX HAS ALREADY CONNECTED WIRES AND OVERWRITE NETWORK (WHEN DRAGGING A JOINT OVER AN EXISTING WIRE)
	this->splitWire->disconnect();
    this->splitWire->getNetwork()->removeWire(this->splitWire.get());
    this->wireContainer->removeWire(this->splitWire.get());

    this->joint->setNetwork(this->splitWire->getNetwork());
    this->splitWire->getNetwork()->joints.push_back(this->joint.get());
    this->splitWire->getNetwork()->wires.push_back(this->createdWires[0].get());
    this->splitWire->getNetwork()->wires.push_back(this->createdWires[1].get());

	this->createdWires[0]->connect();
	this->createdWires[1]->connect();

    this->compContainer->addComponent(this->joint);
    this->wireContainer->addWire(this->createdWires[0]);
    this->wireContainer->addWire(this->createdWires[1]);
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
        this->splitWire->setNetwork(this->joint->getNetwork());
    }
    this->createdWires[0]->disconnect();
    this->createdWires[1]->disconnect();
    this->joint->getNetwork()->removeWire(this->createdWires[0].get());
    this->joint->getNetwork()->removeWire(this->createdWires[1].get());
    this->wireContainer->removeWire(this->createdWires[0].get());
    this->wireContainer->removeWire(this->createdWires[1].get());

    this->joint->getNetwork()->removeJoint(this->joint.get());
    this->compContainer->removeComponent(this->joint.get());

	this->splitWire->connect();
    this->splitWire->getNetwork()->wires.push_back(this->splitWire.get());
    this->wireContainer->addWire(this->splitWire);
}
