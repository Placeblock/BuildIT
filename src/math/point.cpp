#include "point.h"

void Point::add(Point point) {
    this->x += point.x;
    this->y += point.y;
}
