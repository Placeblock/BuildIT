//
// Created by felix on 9/17/24.
//

#include "aABBInteractable.h"

bool AABBInteractable::intersects(glm::vec2 pos) const {
    return pos.x >= this->boundingBox.start.x && pos.y >= this->boundingBox.start.y &&
           pos.x <= this->boundingBox.start.x + this->boundingBox.size.x &&
           pos.y <= this->boundingBox.start.y + this->boundingBox.size.y;
}

bool AABBInteractable::intersects(BoundingBox otherBB) const {
    return this->boundingBox.start.x + this->boundingBox.size.x >= otherBB.start.x &&
           otherBB.start.x + otherBB.size.x >= this->boundingBox.start.x &&
           this->boundingBox.start.y + this->boundingBox.size.y >= otherBB.start.y &&
           otherBB.start.y + otherBB.size.y >= this->boundingBox.start.y;
}

AABBInteractable::AABBInteractable(BoundingBox boundingBox) : boundingBox(boundingBox) {}

void AABBInteractable::updateBoundingBox(BoundingBox newBB) {
    this->boundingBox = newBB;
}

void AABBInteractable::updateBoundingBoxPos(glm::vec2 pos) {
    this->boundingBox.start = pos;
}
