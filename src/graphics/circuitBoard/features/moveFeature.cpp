//
// Created by felix on 8/31/24.
//

#include <GLFW/glfw3.h>
#include "moveFeature.h"

void MoveFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            Component *colliding = this->collisionDetection->getColliding(relPos);
            if (colliding != nullptr) {
                this->movingComponents.insert(colliding);
            }
            if ((colliding != nullptr || mods & GLFW_MOD_SHIFT) && this->selectionFeature != nullptr) {
                this->movingComponents.insert(this->selectionFeature->getComponents()->begin(),
                                              this->selectionFeature->getComponents()->end());
            }
            RendererAddVisitor addVisitor{&this->visRenderers};
            for (const auto &component: this->movingComponents) {
                component->visit(&addVisitor);
            }
        } else {
            RendererRemoveVisitor removeVisitor{&this->visRenderers};
            for (const auto &component: this->movingComponents) {
                component->visit(&removeVisitor);
            }
            this->movingComponents.clear();
            this->moveDelta = {};
        }
    }

}

void MoveFeature::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    if (this->movingComponents.empty()) return;
    this->moveDelta += delta;
    RendererMoveVisitor moveVisitor{&this->visRenderers, this->moveDelta};
    for (const auto &component: this->movingComponents) {
        component->visit(&moveVisitor);
    }
}

MoveFeature::MoveFeature(CollisionDetection<Component> *collisionDetection, SelectionFeature *selectionFeature, FontRenderer *fontRenderer) :
    collisionDetection(collisionDetection), selectionFeature(selectionFeature), visRenderers(fontRenderer) {

}
