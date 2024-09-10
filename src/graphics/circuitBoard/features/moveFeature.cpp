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
            this->moveDelta = this->cursorFeature->getHoveringCellDelta();
            this->updateMovingComponents();
        } else {
            for (const auto &component: this->movingComponents) {
                component->move(component->getPos() + this->moveDelta);
            }
            RendererRemoveVisitor removeVisitor{&this->visRenderers, this->moveDelta};
            for (const auto &component: this->movingComponents) {
                component->visit(&removeVisitor);
            }
            this->movingComponents.clear();
            this->moveDelta = {};
        }
    }
}

void MoveFeature::updateMovingComponents() {
    RendererMoveVisitor moveVisitor{&this->visRenderers, this->moveDelta};
    for (const auto &component: this->movingComponents) {
        component->visit(&moveVisitor);
    }
}


void MoveFeature::notify(const CursorEvent &data) {
    if (this->movingComponents.empty()) return;
    this->moveDelta += data.delta;
}

MoveFeature::MoveFeature(CollisionDetection<Component> *collisionDetection, SelectionFeature *selectionFeature, CursorFeature *cursorFeature, FontRenderer *fontRenderer) :
    collisionDetection(collisionDetection), selectionFeature(selectionFeature), cursorFeature(cursorFeature), visRenderers(fontRenderer) {
    cursorFeature->subscribe(this);
}