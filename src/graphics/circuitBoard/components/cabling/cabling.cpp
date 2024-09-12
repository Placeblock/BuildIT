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
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(),
                                   [&pos](const auto& wire) {
                                       const glm::vec2 left = wire->start->getPos() - pos;
                                       const glm::vec2 right = wire->end->getPos() - pos;
                                       return left.x*right.y - left.y*right.x == 0 &&
                                              left.x*right.x + left.y*right.y < 0;
                                   });
    if (iter != this->wires.end()) return *iter;
    return nullptr;
}

void Cabling::notify(const MoveEvent &event) {
    if (Joint *joint = dynamic_cast<Joint*>(event.movable)) {
        if (this->posMap.contains(joint->getPos()) &&
            this->posMap[joint->getPos()] == joint) { // When moving multiple this could be false
            this->posMap.erase(joint->getPos());
        }
        this->posMap[event.newPos] = joint;
    }
}

void Cabling::notify(const ComponentAddEvent& data) {
    if (Joint *joint = dynamic_cast<Joint*>(data.component)) {
        this->posMap[joint->getPos()] = joint;
        joint->Movable::subscribe(this);
    }
}

void Cabling::notify(const ComponentRemoveEvent& data) {
    if (Joint *joint = dynamic_cast<Joint*>(data.component)) {
        this->posMap.erase(joint->getPos());
        joint->Movable::unsubscribe(this);
    }
}

void Cabling::notify(const WireAddEvent &data) {
    this->wires.insert(data.wire);
}

void Cabling::notify(const WireRemoveEvent &data) {
    this->wires.erase(data.wire);
}
