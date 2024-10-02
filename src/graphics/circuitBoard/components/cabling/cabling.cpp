//
// Created by felix on 8/28/24.
//

#include <algorithm>

#include "cabling.h"
#include "joint.h"


Joint* Cabling::getJoint(glm::vec2 cell) const {
    if (this->posMap.contains(cell)) {
        return this->posMap.at(cell);
    }
    return nullptr;
}

Wire* Cabling::getWire(glm::vec2 pos) {
    const auto iter = std::ranges::find_if(this->wires,
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
    if (!event.before) return;
    if (auto *joint = dynamic_cast<Joint*>(event.movable)) {
        if (this->posMap.contains(joint->getPos() / 32.0f) &&
            this->posMap[joint->getPos() / 32.0f] == joint) { // When moving multiple this could be false
            this->posMap.erase(joint->getPos() / 32.0f);
        }
        this->posMap[(joint->getPos() + event.delta) / 32.0f] = joint;
    }
}

void Cabling::notify(const ComponentAddEvent& data) {
    if (auto *joint = dynamic_cast<Joint*>(data.component)) {
        this->posMap[joint->getPos() / 32.0f] = joint;
        joint->Movable::subscribe(this);
    } else if (auto *wire = dynamic_cast<Wire*>(data.component)) {
        this->wires.insert(wire);
    }
}

void Cabling::notify(const ComponentRemoveEvent& data) {
    if (auto *joint = dynamic_cast<Joint*>(data.component)) {
        this->posMap.erase(joint->getPos() / 32.0f);
        joint->Movable::unsubscribe(this);
    } else if (auto *wire = dynamic_cast<Wire*>(data.component)) {
        this->wires.erase(wire);
    }
}