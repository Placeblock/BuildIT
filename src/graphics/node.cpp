//
// Created by felix on 7/31/24.
//

#include "node.h"
#include <raymath.h>

Graphics::Node::Node(Vector2 pos) {
    this->pos = pos;
}

Graphics::Pin::Pin(Sim::Reference simPin, Vector2 center) {
    this->simPin = simPin;
    this->center = center;
}

void Graphics::Pin::draw(Color color) const {
    DrawRectangleV(Vector2SubtractValue(this->center, 10), Vector2(20, 20), color);
}
