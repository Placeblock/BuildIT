//
// Created by felix on 7/31/24.
//

#include "notGateNode.h"

#include <raylib.h>
#include <raymath.h>

void Graphics::NotGateNode::render(int lod) const {
    if (lod <= 1) {
        DrawCircleV(getWorldPos(Vector2Add(this->pos, Vector2(0, 2))), 10, this->simNode->getInput(0) ? GREEN : RED);
        DrawCircleV(getWorldPos(Vector2Add(this->pos, Vector2(4, 2))), 10, this->simNode->getOutput(0) ? GREEN : RED);
    }
    DrawTriangle(
            getWorldPos(this->pos),
            getWorldPos(Vector2Add(this->pos, Vector2(0, 4))),
            Vector2Subtract(getWorldPos(Vector2Add(this->pos, Vector2(4, 2))), Vector2(20, 0)),
            BLUE
    );
    if (lod <= 2) {
        DrawCircleV(Vector2Subtract(getWorldPos(Vector2Add(this->pos, Vector2(4, 2))), Vector2(10, 0)), 10, BLUE);
    }
}

Graphics::NotGateNode::NotGateNode(Vector2 pos, Sim::NotGate *simNode) : GateNode(pos, simNode) {

}