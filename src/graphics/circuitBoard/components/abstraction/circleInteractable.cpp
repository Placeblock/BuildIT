//
// Created by felix on 9/17/24.
//

#include "circleInteractable.h"
#include "glm/geometric.hpp"

bool CircleInteractable::intersects(glm::vec2 pos) const {
    return glm::length(pos - this->getCenter()) <= this->radius;
}

bool CircleInteractable::intersects(BoundingBox otherBB) const {
    glm::vec2 closestPoint = otherBB.clamp(this->getCenter());
    return glm::length(closestPoint - this->getCenter()) <= this->radius;
}

CircleInteractable::CircleInteractable(float radius) : radius(radius) {

}
