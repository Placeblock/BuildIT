//
// Created by felix on 9/6/24.
//

#include "positionable.h"


Positionable::Positionable(glm::vec2 pos) : pos(pos) {}

glm::vec2 Positionable::getPos() const {
    return this->pos;
}

bool Positionable::intersects(glm::vec2 pos) {
    const glm::vec2 relative = pos - this->pos;
    return relative.x >= this->boundingBox.start.x && relative.y >= this->boundingBox.start.y &&
           relative.x <= this->boundingBox.start.x + this->boundingBox.size.x &&
           relative.x <= this->boundingBox.start.y + this->boundingBox.size.y;
}