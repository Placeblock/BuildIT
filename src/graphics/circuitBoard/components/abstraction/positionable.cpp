//
// Created by felix on 9/6/24.
//

#include "positionable.h"

Positionable::Positionable(glm::vec2 pos, glm::vec2 size)
    : pos(pos), size(size) {}

glm::vec2 Positionable::getPos() const {
    return this->pos;
}