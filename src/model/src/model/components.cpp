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
