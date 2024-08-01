//
// Created by felix on 7/31/24.
//

#include "andGateNode.h"

#include <raylib.h>
#include <raymath.h>

void Graphics::AndGateNode::render(int lod) const {
    if (lod <= 1) {
        DrawCircleV(getWorldPos(Vector2Add(this->pos, Vector2(0, 1))), 10, this->simNode->getInput(0) ? GREEN : RED);
        DrawCircleV(getWorldPos(Vector2Add(this->pos, Vector2(0, 3))), 10, this->simNode->getInput(1) ? GREEN : RED);
        DrawCircleV(getWorldPos(Vector2Add(this->pos, Vector2(4, 2))), 10, this->simNode->getOutput(0) ? GREEN : RED);
    }
    if (lod <= 2) {
        DrawRectangleV(getWorldPos(this->pos), Vector2(64, 128), BLUE);
        DrawCircleSector(getWorldPos(Vector2Add(this->pos, Vector2(2, 2))), 64, -90, 90, 8, BLUE);
    } else {
        DrawRectangleV(getWorldPos(this->pos), Vector2(128, 128), BLUE);
    }
}

Graphics::AndGateNode::AndGateNode(Vector2 pos, Sim::AndGate *simNode) : GateNode(pos, simNode) {

}