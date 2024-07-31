//
// Created by felix on 7/31/24.
//

#include "gateNode.h"
#include "node.h"
#include <raylib.h>
#include <iostream>

void Graphics::GateNode::render(int lod) const {
    DrawRectangle(this->x, this->y, this->w, this->h, BLUE);
    if (lod < 1) {
        for (int i = 0; i < this->simNode->parents.size(); i++) {
            const int dy = this->h/(this->simNode->parents.size()+1);
            DrawRectangle(this->x-20, this->y+dy*(i+1)-10, 20, 20, this->simNode->getInput(i) ? GREEN : RED);
        }
        for (int i = 0; i < this->simNode->children.size(); i++) {
            const int dy = this->h/(this->simNode->children.size()+1);
            DrawRectangle(this->x+this->w, this->y+dy*(i+1)-10, 20, 20, this->simNode->getOutput(i) ? GREEN : RED);
        }
    }
}

void Graphics::GateNode::updateRendered(int lod, Vector2 offset) {
    if (lod > 1) {
        this->rendered = false;
        return;
    }
    this->rendered = this->x > -offset.x-this->w && this->y > -offset.y-this->h && this->x < -offset.x+GetScreenWidth() && this->y < -offset.y+GetScreenHeight();
}

Graphics::GateNode::GateNode(int x, int y, Sim::Node *simNode) : Node(x, y) {
    this->simNode = simNode;
}
