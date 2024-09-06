//
// Created by felix on 8/28/24.
//

#include "wires.h"


Joint* Wires::getJoint(intVec2 cell) const {
    if (const auto result = this->cellMap.find(cell); result != this->cellMap.end()) {
        return result->second;
    }
    return nullptr;
}

Wire* Wires::getWire(glm::vec2 cell) {
    const auto iter = std::find_if(this->wireMap.begin(), this->wireMap.end(),
                                   [&cell](const auto& pair) {
                                       const glm::vec2 left = pair.first->start->getPos() - cell;
                                       const glm::vec2 right = pair.first->end->getPos() - cell;
                                       return left.x*right.y - left.y*right.x == 0 &&
                                              left.x*right.x + left.y*right.y < 0;
                                   });
    if (iter != this->wireMap.end()) return iter->first;
    return nullptr;
}

Network* Wires::getNetwork(Joint* joint) {
    if (const auto result = this->jointMap.find(joint); result != this->jointMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Wires::setNetwork(Joint *joint, Network *network) {
    joint->network = network;
    this->jointMap[joint] = network;
    network->joints.insert(joint);
}

void Wires::setNetwork(Wire *wire, Network *network) {
    wire->network = network;
    this->wireMap[wire] = network;
    network->wires.insert(wire);
}

void Wires::update(const MoveEvent &event, Joint *joint) {
    if (this->cellMap.contains(joint->getPos()) && this->cellMap[joint->getPos()] == joint) { // When moving multiple this could be false
        this->cellMap.erase(joint->getPos());
    }
    this->cellMap[event.newPos] = joint;
}

void Wires::update(const JointAddEvent &data) {
    Joint *joint = data.joint;
    this->jointMap[joint] = joint->network;
    this->cellMap[joint->getPos()] = joint;
    joint->network->joints.insert(joint);
    joint->Movable::subscribe(this->addSubject(joint));
}

void Wires::update(const JointRemoveEvent &data) {
    Joint *joint = data.joint;
    this->jointMap.erase(joint);
    this->cellMap.erase(joint->getPos());
    joint->network->removeJoint(joint);
    joint->Movable::unsubscribe(this->removeSubject(joint));
}

void Wires::update(const WireAddEvent &data) {
    Wire *wire = data.wire;
    this->wireMap[wire] = wire->network;
    wire->network->wires.insert(wire);
}

void Wires::update(const WireRemoveEvent &data) {
    Wire *wire = data.wire;
    this->wireMap.erase(wire);
    wire->network->removeWire(wire, true);
}