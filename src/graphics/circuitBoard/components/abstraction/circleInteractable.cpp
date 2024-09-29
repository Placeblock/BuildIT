//
// Created by felix on 9/17/24.
//

#include "circleInteractable.h"

#include <utility>
#include "glm/geometric.hpp"

bool CircleInteractable::intersects(const glm::vec2 pos) const {
    return glm::length(pos - this->getCenter()) <= this->radius;
}

bool CircleInteractable::intersects(const BoundingBox otherBB) const {
    const glm::vec2 closestPoint = otherBB.clamp(this->getCenter());
    return glm::length(closestPoint - this->getCenter()) <= this->radius;
}

CircleInteractable::CircleInteractable(std::string cnamespace, std::string ckey, const float radius)
    : Interactable(std::move(cnamespace), std::move(ckey)), radius(radius) {

}

CircleInteractable::CircleInteractable(const CircleInteractable &other) = default;