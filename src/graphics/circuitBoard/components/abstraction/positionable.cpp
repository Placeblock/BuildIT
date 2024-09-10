//
// Created by felix on 9/6/24.
//

#include "positionable.h"

Positionable::Positionable(glm::vec2 pos, glm::vec2 size) : pos(pos), Boundable(Positionable::calcBoundingBox(pos, size)) {}

glm::vec2 Positionable::getPos() const {
    return this->pos;
}

BoundingBox Positionable::calcBoundingBox(glm::vec2 pos, glm::vec2 size) {
    return {pos - 5.0f, size + 10.0f};
}
