//
// Created by felix on 8/31/24.
//

#include <GLFW/glfw3.h>
#include "moveFeature.h"

void MoveFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    Component *colliding = this->collisionDetection->getColliding(relPos);
    if (colliding != nullptr) {
        this->movingComponents.insert(colliding);
    }
    if ((colliding != nullptr || mods & GLFW_MOD_SHIFT) && this->selectionFeature != nullptr) {
        this->movingComponents.insert(this->selectionFeature->getComponents()->begin(),
                                      this->selectionFeature->getComponents()->end());
    }
}

void MoveFeature::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
}

MoveFeature::MoveFeature(CollisionDetection<Component> *collisionDetection, SelectionFeature *selectionFeature) :
    collisionDetection(collisionDetection), selectionFeature(selectionFeature) {

}
