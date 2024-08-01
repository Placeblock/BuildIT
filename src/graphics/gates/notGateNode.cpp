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
    DrawRectangle(this->pos.x-20, this->pos.y+this->h/2-10, 20, 20, this->simNode->getInput(0) ? GREEN : RED);
    DrawRectangle(this->pos.x+this->w, this->pos.y+this->h/2-10, 20, 20, this->simNode->getOutput(0) ? GREEN : RED);
}

Graphics::NotGateNode::NotGateNode(Vector2 pos, Sim::NotGate *simNode) : GateNode(pos, simNode) {

}