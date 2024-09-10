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
        joint->getNetwork()->joints.push_back(joint);
        joint->Movable::subscribe(this);
    }
}

void Cabling::notify(const ComponentRemoveEvent& data) {
    if (Joint *joint = dynamic_cast<Joint*>(data.component)) {
        this->posMap.erase(joint->getPos());
        joint->getNetwork()->removeJoint(joint);
        joint->Movable::unsubscribe(this);
    }
}
