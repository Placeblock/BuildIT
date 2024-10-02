//
// Created by felix on 9/17/24.
//

#include "aABBInteractable.h"

#include <utility>

bool AABBInteractable::intersects(const glm::vec2 pos) const {
    return this->boundingBox.contains(pos);
}

bool AABBInteractable::intersects(const BoundingBox otherBB) const {
    return this->boundingBox.start.x + this->boundingBox.size.x >= otherBB.start.x &&
           otherBB.start.x + otherBB.size.x >= this->boundingBox.start.x &&
           this->boundingBox.start.y + this->boundingBox.size.y >= otherBB.start.y &&
           otherBB.start.y + otherBB.size.y >= this->boundingBox.start.y;
}

AABBInteractable::AABBInteractable(std::string cnamespace, std::string ckey, const BoundingBox boundingBox)
    : Interactable(std::move(cnamespace), std::move(ckey)), boundingBox(boundingBox) {}

AABBInteractable::AABBInteractable(const AABBInteractable &other) : Interactable(other), boundingBox(other.boundingBox) {

}

void AABBInteractable::updateBoundingBox(const BoundingBox newBB) {
    this->boundingBox = newBB;
}

void AABBInteractable::updateBoundingBoxPos(const glm::vec2 pos) {
    this->boundingBox.start = pos;
}
