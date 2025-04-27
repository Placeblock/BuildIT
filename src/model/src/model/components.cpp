//
// Created by felix on 23.04.25.
//

#include "model/components.hpp"


Models::Position & Models::Position::operator+=(const Position &rhs) & {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Models::Position Models::Position::operator+(Position lhs, const Position &rhs) const {
    lhs += rhs;
    return lhs;
}

Models::Position & Models::Position::operator*=(int i) & {
    x *= i;
    y *= i;
    return *this;
}
