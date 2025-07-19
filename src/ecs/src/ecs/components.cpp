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

void rotation::apply(position &pos) const {
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

position pin::get_abs(const position &componentPos, const rotation &rot) const {
    position abs_pos = this->pos;
    rot.apply(abs_pos);
    return abs_pos + componentPos;
}

position &position::operator+=(const position &other) {
    x += other.x;
    y += other.y;
    return *this;
}

position &position::operator*=(const int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

bool position::operator==(const position &other) const {
    return x == other.x && y == other.y;
}
