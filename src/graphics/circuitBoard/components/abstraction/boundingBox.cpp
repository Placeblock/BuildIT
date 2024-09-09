//
// Created by felix on 9/9/24.
//

#include "boundingBox.h"

bool Boundable::intersects(glm::vec2 pos) const {
    return pos.x >= this->boundingBox.start.x && pos.y >= this->boundingBox.start.y &&
            pos.x <= this->boundingBox.start.x + this->boundingBox.size.x &&
            pos.x <= this->boundingBox.start.y + this->boundingBox.size.y;
}

Boundable::Boundable(BoundingBox bb) : boundingBox(bb) {}
