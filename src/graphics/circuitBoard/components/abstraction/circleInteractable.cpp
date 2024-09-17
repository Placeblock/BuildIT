//
// Created by felix on 9/17/24.
//

#include "circleInteractable.h"
#include "glm/geometric.hpp"

bool CircleInteractable::intersects(glm::vec2 pos) const {
    return glm::length(pos - this->center) <= this->radius;
}

bool CircleInteractable::intersects(BoundingBox otherBB) const {
    glm::vec2 closestPoint = otherBB.clamp(this->center);
    return glm::length(closestPoint - this->center) <= this->radius;
}

CircleInteractable::CircleInteractable(glm::vec2 center, float radius) : center(center), radius(radius) {

}

void CircleInteractable::setCenter(glm::vec2 newCenter) {
    this->center = newCenter;
}
