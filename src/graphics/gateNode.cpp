//
// Created by felix on 7/31/24.
//

#include "gateNode.h"
#include <raylib.h>

void Graphics::GateNode::render(int lod) const {
    DrawRectangle(this->x, this->y, 20, 20, GREEN);
}

bool Graphics::GateNode::updateRedered(int lod, Vector2 offset) {
    this->rendered = this->x > offset.x-this->w && this->y > offset.y-this->h && this->x < offset.x+GetScreenWidth() && this->y < offset.y+GetScreenHeight();
}
