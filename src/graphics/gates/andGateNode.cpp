//
// Created by felix on 7/31/24.
//

#include "andGateNode.h"

#include <raylib.h>
#include <raymath.h>

void Graphics::AndGateNode::render(int lod) const {
    DrawRectangleV(this->pos, Vector2(this->w/2, this->h), BLUE);
    DrawCircleSector(Vector2Add(this->pos, Vector2(this->w/2, this->h/2)), this->h/2, -90, 90, 8, BLUE);
    DrawRectangle(this->pos.x-20, this->pos.y+this->h/3-10, 20, 20, this->simNode->getInput(0) ? GREEN : RED);
    DrawRectangle(this->pos.x-20, this->pos.y+this->h/3*2-10, 20, 20, this->simNode->getInput(1) ? GREEN : RED);
    DrawRectangle(this->pos.x+this->w, this->pos.y+this->h/2-10, 20, 20, this->simNode->getOutput(0) ? GREEN : RED);
}

Graphics::AndGateNode::AndGateNode(Vector2 pos, Sim::AndGate *simNode) : GateNode(pos, simNode) {

}