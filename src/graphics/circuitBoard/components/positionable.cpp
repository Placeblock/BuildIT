//
// Created by felix on 9/1/24.
//

#include "positionable.h"

glm::vec2 Positionable::getCell() const {
    return this->cell;
}

bool Positionable::intersects(glm::vec2 pos) {
    const glm::vec2 relative = pos - this->cell;
    return relative.x >= this->boundingBox.start.x && relative.y >= this->boundingBox.start.y &&
           relative.x <= this->boundingBox.start.x + this->boundingBox.size.x &&
           relative.x <= this->boundingBox.start.y + this->boundingBox.size.y;
}