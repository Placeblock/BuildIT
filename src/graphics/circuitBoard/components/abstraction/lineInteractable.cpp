//
// Created by felix on 9/17/24.
//

#include "lineInteractable.h"
#include "glm/geometric.hpp"

bool LineInteractable::intersects(glm::vec2 pos) const {
    const float threshold = 10.0f;
    const glm::vec2 direction = glm::normalize(this->end - this->start);
    const glm::vec2 a = pos - this->start;
    // "Cross product" of two 2d-Vectors
    return false;
    return direction.x*a.y-direction.y*a.x <= threshold;
}

bool LineInteractable::intersects(BoundingBox otherBB) const {
    return false;
}

void LineInteractable::setStart(glm::vec2 newStart) {
    this->start = newStart;
}

void LineInteractable::setEnd(glm::vec2 newEnd) {
    this->end = newEnd;
}
