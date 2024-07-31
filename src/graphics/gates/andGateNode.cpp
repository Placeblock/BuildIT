//
// Created by felix on 7/31/24.
//

#include "andGateNode.h"

#include <raylib.h>
#include <raymath.h>

void Graphics::AndGateNode::render(int lod) const {
    DrawRectangleV(this->pos, Vector2(this->w/2, this->h), BLUE);
    DrawCircleSector(Vector2Add(this->pos, Vector2(this->w/2, this->h/2)), this->h/2, -90, 90, 8, BLUE);
    this->inputPins[0].draw(this->simNode->getInput(0) ? GREEN : RED);
    this->inputPins[1].draw(this->simNode->getInput(0) ? GREEN : RED);
    this->outputPins[0].draw(this->simNode->getOutput(0) ? GREEN : RED);
}

Graphics::AndGateNode::AndGateNode(Vector2 pos, Sim::AndGate *simNode) : GateNode(pos, simNode) {

}

void Graphics::AndGateNode::updatePinPosition() {
    this->inputPins[0].center = Vector2(this->pos.x-10, this->pos.y+this->h/3);
    this->inputPins[1].center = Vector2(this->pos.x-10, this->pos.y+this->h/3*2);
    this->outputPins[0].center = Vector2(this->pos.x+this->w+10, this->pos.y+this->h/2);
}
