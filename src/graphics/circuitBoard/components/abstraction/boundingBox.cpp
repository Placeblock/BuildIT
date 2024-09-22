//
// Created by felix on 9/17/24.
//

#include "boundingBox.h"
#include "glm/common.hpp"

BoundingBox calcBoundingBox(glm::vec2 pos, glm::vec2 size) {
    return {pos - 5.0f, size + 10.0f};
}

glm::vec2 BoundingBox::clamp(glm::vec2 point) const {
    return glm::clamp(point, this->start, this->start + this->size);
}

glm::vec2 BoundingBox::getCenter() const {
    return this->start + this->start / 2.0f;
}

bool BoundingBox::contains(glm::vec2 point) const {
    return point.x >= this->start.x && point.x <= this->start.x + this->size.x
        && point.y >= this->start.y && point.y <= this->start.y + this->size.y;
}
