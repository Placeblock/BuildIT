//
// Created by felix on 8/31/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "moveFeature.h"
#include "graphics/circuitBoard/data/selection.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/circuitBoard/history/actions/moveComponentAction.h"
#include "graphics/circuitBoard/history/history.h"

void MoveFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
            Component *colliding = this->collisionDetection->getColliding(cursorPos);
            if (colliding != nullptr) {
                this->movingComponents.insert(colliding);
            }
            if ((colliding != nullptr || mods & GLFW_MOD_SHIFT) && this->selectionAccessor != nullptr) {
                this->movingComponents.insert(this->selectionAccessor->getComponents()->begin(),
                                              this->selectionAccessor->getComponents()->end());
            }
            if (this->movingComponents.empty()) return;
            RendererAddVisitor addVisitor{&this->visRenderers};
            for (const auto &component: this->movingComponents) {
                component->visit(&addVisitor);
            }
            this->moveDelta = this->cursorFeature->getHoveringCellDelta();
            this->startCell = this->cursorFeature->getHoveringCell();
            this->updateMovingComponents();
        } else {
            if (this->cursorFeature->getHoveringCell() != this->startCell) {
                History::startBatch(this->history);
                intVec2 cellDelta = this->cursorFeature->getHoveringCell() - this->startCell;
                for (const auto &component: this->movingComponents) {
                    glm::vec2 newPos = component->getPos() + glm::vec2(cellDelta * 32);
                    std::unique_ptr<Action> dAction = std::make_unique<MoveComponentAction>(component, newPos);
                    History::dispatch(this->history, dAction);
                }
                History::endBatch(this->history);
            }
            this->endMove();
        }
    }
}

void MoveFeature::endMove() {
    RendererRemoveVisitor removeVisitor{&this->visRenderers};
    for (const auto &component: this->movingComponents) {
        component->visit(&removeVisitor);
    }
    this->movingComponents.clear();
    this->moveDelta = {};
    this->startCell = {};
}


void MoveFeature::notify(const HistoryChangeEvent &data) {
    this->endMove();
}

void MoveFeature::updateMovingComponents() {
    for (const auto &component: this->movingComponents) {
        glm::vec2 newPos = component->getPos() + this->moveDelta;
        RendererMoveVisitor moveVisitor{&this->visRenderers, newPos};
        component->visit(&moveVisitor);
    }
}


void MoveFeature::notify(const CursorEvent &data) {
    if (this->movingComponents.empty()) return;
    this->moveDelta += data.delta;
    this->updateMovingComponents();
}

MoveFeature::MoveFeature(Programs *programs, History *history, CollisionDetection<Component> *collisionDetection,
                         SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature, FontRenderer *fontRenderer) :
                         Renderable(programs), history(history), collisionDetection(collisionDetection), selectionAccessor(selectionAccessor),
                         cursorFeature(cursorFeature), visRenderers(fontRenderer) {
    cursorFeature->subscribe(this);
}

void MoveFeature::render() {
    this->visRenderers.render(this->programs);
}