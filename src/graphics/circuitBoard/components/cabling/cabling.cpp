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
