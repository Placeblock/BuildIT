//
// Created by felix on 9/17/24.
//

#include "lineInteractable.h"
#include "glm/geometric.hpp"

bool LineInteractable::intersects(glm::vec2 pos) const {
    const glm::vec2 u = this->getStartPos() - pos;
    const glm::vec2 v = this->getEndPos() - pos;
    return u.x*v.y-u.y*v.x == 0 && u.x*v.x+u.y*v.y <= 0 && glm::length(u) > 10 && glm::length(v) > 10;
}

bool LineInteractable::intersects(BoundingBox otherBB) const {
    otherBB.contains(this->getStartPos()) && otherBB.contains(this->getEndPos());
}
