//
// Created by felix on 9/17/24.
//

#include "lineInteractable.h"

#include <utility>
#include "glm/geometric.hpp"

bool LineInteractable::intersects(const glm::vec2 pos) const {
    const glm::vec2 u = this->getStartPos() - pos;
    const glm::vec2 v = this->getEndPos() - pos;
    return u.x*v.y-u.y*v.x == 0 && u.x*v.x+u.y*v.y <= 0 && glm::length(u) > 10 && glm::length(v) > 10;
}

bool LineInteractable::intersects(const BoundingBox otherBB) const {
    return otherBB.contains(this->getStartPos()) && otherBB.contains(this->getEndPos());
}

LineInteractable::LineInteractable(std::string cnamespace, std::string ckey)
    : Interactable(std::move(cnamespace), std::move(ckey)) {

}

LineInteractable::LineInteractable(const LineInteractable &other) = default;