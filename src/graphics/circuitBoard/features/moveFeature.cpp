//
// Created by felix on 8/31/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "moveFeature.h"
#include "graphics/circuitBoard/selection/selection.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/circuitBoard/history/actions/moveComponentAction.h"
#include "graphics/circuitBoard/history/history.h"

void MoveFeature::onMouseAction(glm::vec2 relPos, const int button, const int action, const int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            const glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
            Component *colliding = this->collisionDetection->getColliding(cursorPos);
            if (colliding != nullptr) {
                if (dynamic_cast<Joint*>(colliding)) {
                    if (!(mods & GLFW_MOD_SHIFT)) return;
                }
                if (const auto movable = dynamic_cast<Movable*>(colliding)) {
                    this->movingComponents.insert(movable);
                }
            }
            if ((colliding != nullptr || mods & GLFW_MOD_SHIFT) && this->selectionAccessor != nullptr) {
                for (const auto &selectable: *this->selectionAccessor->getSelected()) {
                    if (auto movable = dynamic_cast<Movable*>(selectable)) {
                        this->movingComponents.insert(movable);
                    }
                }
            }
            if (this->movingComponents.empty()) return;
            RendererAddVisitor addVisitor{&this->visRenderers};
            for (const auto &component: this->movingComponents) {
                component->visit(&addVisitor);
                if (const auto *joint = dynamic_cast<Joint*>(component)) {
                    for (const auto &wire: joint->wires) {
                        this->visRenderers.cablingRenderer.addWire(wire, false);
                    }
                }
            }
            this->moveDelta = this->cursorFeature->getHoveringCellDelta();
            this->startCell = this->cursorFeature->getHoveringCell();
            this->updateMovingComponents(this->moveDelta);
        } else {
            if (this->cursorFeature->getHoveringCell() != this->startCell) {
                History::startBatch(this->history);
                const intVec2 cellDelta = this->cursorFeature->getHoveringCell() - this->startCell;
                for (const auto &component: this->movingComponents) {
                    std::unique_ptr<Action> dAction = std::make_unique<MoveComponentAction>(component, cellDelta * 32);
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
        if (const auto *joint = dynamic_cast<Joint*>(component)) {
            for (const auto &wire: joint->wires) {
                this->visRenderers.cablingRenderer.removeWire(wire, false);
            }
        }
    }
    this->movingComponents.clear();
    this->moveDelta = {};
    this->startCell = {};
}


void MoveFeature::notify(const HistoryChangeEvent &data) {
    this->endMove();
}

void MoveFeature::updateMovingComponents(const glm::vec2 delta) {
    for (const auto &component: this->movingComponents) {
        RendererMoveVisitor moveVisitor{&this->visRenderers, delta};
        component->visit(&moveVisitor);
    }
}


void MoveFeature::notify(const CursorEvent &data) {
    if (this->movingComponents.empty()) return;
    this->moveDelta += data.delta;
    this->updateMovingComponents(data.delta);
}

MoveFeature::MoveFeature(Programs *programs, History *history, CollisionDetection<Interactable> *collisionDetection,
                         SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature, FontRenderer *fontRenderer) :
                         Renderable(programs), history(history), collisionDetection(collisionDetection), selectionAccessor(selectionAccessor),
                         cursorFeature(cursorFeature), visRenderers(fontRenderer) {
    cursorFeature->subscribe(this);
}

void MoveFeature::render() {
    if (!this->movingComponents.empty()) {
        this->visRenderers.render(this->programs);
    }
}
