//
// Created by felix on 8/28/24.
//

#include "wires.h"


Joint* Cabling::getJoint(glm::vec2 cell) const {
    if (const auto result = this->posMap.find(cell); result != this->posMap.end()) {
        return result->second;
    }
    return nullptr;
}

Wire* Cabling::getWire(glm::vec2 pos) {
    const auto iter = std::find_if(this->wireMap.begin(), this->wireMap.end(),
                                   [&pos](const auto& pair) {
                                       const glm::vec2 left = pair.first->start->getPos() - pos;
                                       const glm::vec2 right = pair.first->end->getPos() - pos;
                                       return left.x*right.y - left.y*right.x == 0 &&
                                              left.x*right.x + left.y*right.y < 0;
                                   });
    if (iter != this->wireMap.end()) return iter->first;
    return nullptr;
}

void Cabling::setNetwork(Joint *joint, Network *network) {
    joint->network = network;
    network->joints.insert(joint);
}

void Cabling::setNetwork(Wire *wire, Network *network) {
    wire->network = network;
    this->wireMap[wire] = network;
    network->wires.insert(wire);
}

void Cabling::update(const MoveEvent<Joint> &event, Joint *joint) {
    if (this->posMap.contains(joint->getPos()) && this->posMap[joint->getPos()] == joint) { // When moving multiple this could be false
        this->posMap.erase(joint->getPos());
    }
    this->posMap[event.newPos] = joint;
}

void Cabling::update(const JointAddEvent &data) {
    Joint *joint = data.joint;
    this->posMap[joint->getPos()] = joint;
    joint->network->joints.insert(joint);
    joint->Movable::subscribe(this->addSubject(joint));
}

void Cabling::update(const JointRemoveEvent &data) {
    Joint *joint = data.joint;
    this->posMap.erase(joint->getPos());
    joint->network->removeJoint(joint);
    joint->Movable::unsubscribe(this->removeSubject(joint));
}

void Cabling::update(const WireAddEvent &data) {
    Wire *wire = data.wire;
    this->wireMap[wire] = wire->network;
    wire->network->wires.insert(wire);
}

void Cabling::update(const WireRemoveEvent &data) {
    Wire *wire = data.wire;
    this->wireMap.erase(wire);
    wire->network->removeWire(wire, true);
}