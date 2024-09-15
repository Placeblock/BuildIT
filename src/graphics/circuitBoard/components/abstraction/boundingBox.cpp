//
// Created by felix on 9/9/24.
//

#include "boundingBox.h"

bool Boundable::intersects(glm::vec2 pos) const {
    return pos.x >= this->boundingBox.start.x && pos.y >= this->boundingBox.start.y &&
            pos.x <= this->boundingBox.start.x + this->boundingBox.size.x &&
            pos.y <= this->boundingBox.start.y + this->boundingBox.size.y;
}

Boundable::Boundable(BoundingBox bb) : boundingBox(bb) {}

bool Boundable::intersects(BoundingBox bb) const {
    return this->boundingBox.start.x + this->boundingBox.size.x >= bb.start.x &&
        bb.start.x + bb.size.x >= this->boundingBox.start.x &&
        this->boundingBox.start.y + this->boundingBox.size.y >= bb.start.y &&
        bb.start.y + bb.size.y >= this->boundingBox.start.y;
}
