//
// Created by felix on 8/31/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "moveFeature.h"

#include "graphics/renderers.h"
#include "graphics/circuitBoard/selection/selection.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/circuitBoard/history/actions/moveComponentAction.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/circuitBoard/components/cabling/wire.h"
#include "graphics/circuitBoard/components/cabling/joint.h"

void MoveFeature::onMouseAction(glm::vec2 relPos, const int button, const int action, const int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            const glm::vec2 cursorPos = this->cursorFeature->getHoveringCell() * 32;
            Component *colliding = this->collisionDetection->getColliding(cursorPos);
            if (colliding != nullptr) {
                if (const auto movable = dynamic_cast<Movable*>(colliding)) {
                    this->addMovable(movable);
                }
            }
            if ((colliding != nullptr || mods & GLFW_MOD_SHIFT) && this->selectionAccessor != nullptr) {
                for (const auto &selectable: *this->selectionAccessor->getSelected()) {
                    if (const auto movable = dynamic_cast<Movable*>(selectable)) {
                        this->addMovable(movable);
                    }
                }
            }
            if (this->movables.empty()) return;
            for (const auto &component: this->movables) {
                this->visRenderers.addComponent(component);
            }
            this->moveDelta = this->cursorFeature->getHoveringCellDelta();
            this->startCell = this->cursorFeature->getHoveringCell();
        } else {
            if (this->cursorFeature->getHoveringCell() != this->startCell) {
                History::startBatch(this->history);
                const intVec2 cellDelta = this->cursorFeature->getHoveringCell() - this->startCell;
                for (const auto &component: this->movables) {
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
    for (const auto &component: this->movables) {
        this->visRenderers.removeComponent(component);
    }
    this->movables.clear();
    this->moveDelta = {};
    this->startCell = {};
}


void MoveFeature::notify(const HistoryChangeEvent &data) {
    this->endMove();
}

void MoveFeature::notify(const CursorEvent &data) {
    if (this->movables.empty()) return;
    this->moveDelta += data.delta;
}

MoveFeature::MoveFeature(Programs *programs, History *history, Camera *camera, intVec2 *boardSize, CollisionDetection<Interactable> *collisionDetection,
                         SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature, FontRenderer *fontRenderer) :
                         Renderable(programs), history(history), boardCamera(camera), boardSize(boardSize), collisionDetection(collisionDetection),
                         selectionAccessor(selectionAccessor), cursorFeature(cursorFeature), visRenderers(TestRenderers::getRenderers(fontRenderer)) {
    cursorFeature->subscribe(this);
}

void MoveFeature::render() {
    if (!this->movables.empty()) {
        Camera movingCamera = *this->boardCamera;
        movingCamera.target -= this->moveDelta;
        this->programs->updateProjectionUniforms(*this->boardSize, movingCamera);
        this->visRenderers.render(this->programs);
        this->programs->updateProjectionUniforms(*this->boardSize, *this->boardCamera);
    }
}

void MoveFeature::addMovable(Movable *movable) {
    this->movables.insert(movable);
}
