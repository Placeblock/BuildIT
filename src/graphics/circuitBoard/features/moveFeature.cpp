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
            glm::vec2 boardPos = this->coordinateConverter->screenToWorld(relPos);
            Component *colliding = this->collisionDetection->getColliding(boardPos);
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
            this->updateMovingComponents();
        } else {
            History::startBatch(this->history);
            for (const auto &component: this->movingComponents) {
                std::unique_ptr<Action> dAction = std::make_unique<MoveComponentAction>(component,
                                                                                        component->getPos() + this->moveDelta);
                History::dispatch(this->history, dAction);
            }
            History::endBatch(this->history);
            this->endMove();
        }
    }
}

void MoveFeature::notify(const HistoryChangeEvent &data) {
    this->endMove();
}

void MoveFeature::endMove() {
    RendererRemoveVisitor removeVisitor{&this->visRenderers, this->moveDelta};
    for (const auto &component: this->movingComponents) {
        component->visit(&removeVisitor);
    }
    this->movingComponents.clear();
    this->moveDelta = {};
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

MoveFeature::MoveFeature(Programs *programs, History *history, CollisionDetection<Component> *collisionDetection,
                         SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature, FontRenderer *fontRenderer,
                         CoordinateConverter *coordinateConverter) :
                         Renderable(programs), history(history), collisionDetection(collisionDetection), selectionAccessor(selectionAccessor),
                         cursorFeature(cursorFeature), visRenderers(fontRenderer), coordinateConverter(coordinateConverter) {
    cursorFeature->subscribe(this);
}

void MoveFeature::render() {
    this->visRenderers.render(this->programs);
}
