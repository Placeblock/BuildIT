//
// Created by felix on 23.04.25.
//

#include "ecs/components.hpp"

using namespace buildit::ecs;

rotation &rotation::operator+=(const rotation &rhs) {
    rot = (rot + rhs.rot) % 4;
    return *this;
}

rotation rotation::operator+(const rotation &rhs) const {
    rotation result = *this;
    result += rhs;
    return result;
}

void rotation::apply(glm::vec2 &pos) const {
    if (rot == 1) {
        const int x = pos.x;
        pos.x = -pos.y;
        pos.y = x;
    } else if (rot == 2) {
        pos *= -1;
    } else if (rot == 3) {
        const int y = pos.y;
        pos.y = -pos.x;
        pos.x = y;
    }
}