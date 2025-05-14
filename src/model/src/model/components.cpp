//
// Created by felix on 23.04.25.
//

#include "model/components.hpp"

using namespace Model;

Rotation & Rotation::operator+=(const Rotation &rhs)
{
    rot = (rot+rhs.rot)%4;
    return *this;
}

Rotation Rotation::operator+(const Rotation &rhs) const
{
    Rotation result = *this;
    result += rhs;
    return result;
}

void Rotation::apply(Position &pos) const
{
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

Position Pin::get_abs(const Position& componentPos, const Rotation& rot) const
{
    Position abs_pos = this->pos;
    rot.apply(abs_pos);
    return abs_pos + componentPos;
}

Position &Position::operator+=(const Position &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Position &Position::operator*=(const int scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

bool Position::operator==(const Position &other) const
{
    return x == other.x && y == other.y;
}
