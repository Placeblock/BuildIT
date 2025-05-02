//
// Created by felix on 23.04.25.
//

#include "model/components.hpp"

using namespace Models;

Position & Position::operator+=(const Position &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Position Position::operator+(const Position &rhs) const {
    Position result = *this;
    result += rhs;
    return result;
}

Position & Position::operator*=(int i) {
    x *= i;
    y *= i;
    return *this;
}

bool Position::operator==(const Position &pos) const {
    return x == pos.x && y == pos.y;
}

Rotation & Rotation::operator+=(const Rotation &rhs) {
    rot = (rot+rhs.rot)%4;
    return *this;
}

Rotation Rotation::operator+(const Rotation &rhs) const {
    Rotation result = *this;
    result += rhs;
    return result;
}

void Rotation::apply(Position &pos) const {
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

Position PinSink::getAbs(const Position componentPos, const Rotation *rot) const  {
    Position abs = position;
    if (rot != nullptr) {
        rot->apply(abs);
    }
    abs += componentPos;
    return abs;
}
