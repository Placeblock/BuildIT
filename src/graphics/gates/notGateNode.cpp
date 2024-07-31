//
// Created by felix on 7/31/24.
//

#include "notGateNode.h"

#include <raylib.h>
#include <raymath.h>
#include <iostream>

void Graphics::NotGateNode::render(int lod) const {
    DrawTriangle(this->pos, Vector2Add(this->pos, Vector2(0, this->h)), Vector2Add(this->pos, Vector2(this->w-20, this->h/2)), BLUE);
    DrawCircleV(Vector2Add(this->pos, Vector2(this->w-10, this->h/2)), 10, BLUE);
    this->inputPins[0].draw(this->simNode->getInput(0) ? GREEN : RED);
    this->outputPins[0].draw(this->simNode->getOutput(0) ? GREEN : RED);
}

Graphics::NotGateNode::NotGateNode(Vector2 pos, Sim::NotGate *simNode) : GateNode(pos, simNode) {

}

void Graphics::NotGateNode::updatePinPosition() {
    this->inputPins[0].center = Vector2(this->pos.x-10, this->pos.y+this->h/2);
    this->outputPins[0].center = Vector2(this->pos.x+this->w+10, this->pos.y+this->h/2);
}
