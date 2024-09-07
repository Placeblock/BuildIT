//
// Created by felix on 8/28/24.
//

#include "cabling.h"


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
    joint->setNetwork(network);
    network->joints.push_back(joint);
}

void Cabling::setNetwork(Wire *wire, Network *network) {
    wire->setNetwork(network);
    network->wires.push_back(wire);
}

void Cabling::notify(Joint *joint, const MoveEvent& event) {
    if (this->posMap.contains(joint->getPos()) && this->posMap[joint->getPos()] == joint) { // When moving multiple this could be false
        this->posMap.erase(joint->getPos());
    }
    this->posMap[event.newPos] = joint;
}

void Cabling::notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) {
    Joint *joint = static_cast<Joint*>(data.component);
    this->posMap[joint->getPos()] = joint;
    joint->getNetwork()->joints.push_back(joint);
    joint->Movable::subscribe(this);
}

void Cabling::notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) {
    Joint *joint = static_cast<Joint*>(data.component);
    this->posMap.erase(joint->getPos());
    joint->getNetwork()->removeJoint(joint);
    joint->Movable::unsubscribe(this);
}

void Cabling::notify(Subject<WireAddEvent> *subject, const WireAddEvent& data) {
    data.wire->getNetwork()->wires.push_back(data.wire);
}

void Cabling::notify(Subject<WireRemoveEvent> *subject, const WireRemoveEvent& data) {
    data.wire->getNetwork()->removeWire(data.wire, true);
}

Cabling::Cabling(TypedSubject<ComponentAddEvent, Joint> *jointAddObserver, TypedSubject<ComponentRemoveEvent, Joint> *jointRemoveObserver,
                 TypedSubject<ComponentAddEvent, Wire> *wireAddObserver, TypedSubject<ComponentRemoveEvent, Wire> *wireRemoveObserver) {
    jointAddObserver->subscribe(this);
    jointRemoveObserver->subscribe(this);
    wireAddObserver->subscribe(this);
    wireRemoveObserver->subscribe(this);
}
