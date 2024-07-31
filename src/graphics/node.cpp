//
// Created by felix on 7/31/24.
//

#include "node.h"
#include <raymath.h>

#include <utility>

Graphics::Node::Node(Vector2 pos, std::vector<Pin> inputPins, std::vector<Pin> outputPins) {
    this->pos = pos;
    this->inputPins = std::move(inputPins);
    this->outputPins = std::move(outputPins);
}

Graphics::Pin::Pin(Sim::Reference simPin, Vector2 center) {
    this->simPin = simPin;
    this->center = center;
}

void Graphics::Pin::draw(Color color) const {
    DrawRectangleV(Vector2SubtractValue(this->center, 10), Vector2(20, 20), color);
}
