//
// Created by felix on 7/31/24.
//

#include "gateNode.h"
#include "node.h"
#include <raylib.h>

void Graphics::GateNode::render(int lod) const {
    DrawRectangle(this->x, this->y, this->w, this->h, GREEN);
}

void Graphics::GateNode::updateRendered(int lod, Vector2 offset) {
    if (lod > 1) {
        this->rendered = false;
        return;
    }
    this->rendered = this->x > -offset.x-this->w && this->y > -offset.y-this->h && this->x < -offset.x+GetScreenWidth() && this->y < -offset.y+GetScreenHeight();
}

Graphics::GateNode::GateNode(int x, int y, Simulation::Node simNode) : Node(x, y) {
    this->simNode = simNode;
}
