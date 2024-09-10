//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "modifyCablingFeature.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/circuitBoard/data/selection.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/circuitBoard/history/actions/createWireAction.h"
#include "graphics/circuitBoard/history/actions/insertJointAction.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"

void ModifyCablingFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            Component *colliding = this->collisionDetection->getColliding(relPos);
            if (colliding == nullptr && (!(mods & GLFW_MOD_SHIFT) || this->selectionAccessor->getComponents()->empty())) {
                this->startCable(this->cursorFeature->getHoveringCell());
            }
        } else if (this->wire != nullptr) {
            this->endCable();
        }
    }
}

void ModifyCablingFeature::notify(const CursorEvent &data) {
    if (this->creating) {
        intVec2 endCell = this->calculateEndCell();
        this->wire->end->move(endCell * 32);
    }
}

void ModifyCablingFeature::notify(const HistoryChangeEvent &data) {
    this->visWiresRenderer.removeNetwork(this->visNetwork.get());
    this->creating = false;
}

void ModifyCablingFeature::startCable(intVec2 cell) {
    this->startCell = cell;
    this->wire->start->move(cell * 32);
    this->visWiresRenderer.addNetwork(this->visNetwork.get());
    this->creating = true;
}

void ModifyCablingFeature::endCable() {
    this->visWiresRenderer.removeNetwork(this->visNetwork.get());
    this->creating = false;
    intVec2 endCell = this->calculateEndCell();
    if (endCell != this->startCell) {
        this->createCable(this->startCell, endCell);
    }
}

void ModifyCablingFeature::createCable(intVec2 start, intVec2 end) {
    /**History::startBatch(this->history);
    Joint* startJoint = getJoint(start);
    Joint* endJoint = getJoint(end);
    if (startJoint == nullptr) {
        std::shared_ptr<Network> network = std::make_shared<Network>();
        std::unique_ptr<Joint> joint = std::make_unique<Joint>(start * 32, network.get());
        startJoint = joint.get();
        this->createOrInsertJoint(joint);
    }
    if (endJoint == nullptr) {
        std::shared_ptr<Network> network = std::make_shared<Network>();
        std::unique_ptr<Joint> joint = std::make_unique<Joint>(start * 32, network.get());
        endJoint = joint.get();
        this->createOrInsertJoint(joint);
    }
    std::shared_ptr<Wire> createdWire = std::make_shared<Wire>(startJoint, endJoint, startJoint->getNetwork());
    std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(this->wireContainer, createdWire, false);
    History::dispatch(this->history, dAction);
    History::endBatch(this->history);*/
}

void ModifyCablingFeature::createOrInsertJoint(std::unique_ptr<Joint> &joint) {
    /*std::unique_ptr<Action> dAction;
    Wire *splitWire = getWire(joint->getPos());
    if (splitWire != nullptr) {
        std::shared_ptr<Wire> owningWire = getOwningWire(splitWire);
        dAction = std::make_unique<InsertJointAction>(this->wireContainer, this->componentContainer,
                                                      std::move(joint), owningWire, false);
    } else {
        dAction = std::make_unique<CreateComponentAction>(this->componentContainer, std::move(joint), false);
    }
    History::dispatch(this->history, dAction);*/
}

ModifyCablingFeature::ModifyCablingFeature(Programs *programs, History *history, CollisionDetection<Component> *cd,
                                           SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature,
                                           WireContainer *wireContainer, ComponentContainer *componentContainer)
    : history(history), collisionDetection(cd), selectionAccessor(selectionAccessor),
        cursorFeature(cursorFeature), wireContainer(wireContainer), componentContainer(componentContainer),
      Renderable(programs){

}

intVec2 ModifyCablingFeature::calculateEndCell() {
    return ModifyCablingFeature::calculateEndCell(this->startCell,
                                                  this->cursorFeature->getHoveringCell(),
                                                  this->cursorFeature->getCursorPos());
}


intVec2 ModifyCablingFeature::calculateEndCell(intVec2 startCell, intVec2 hoveringCell, glm::vec2 cursorPos) {
    const float startDistance = glm::distance(glm::vec2(startCell), glm::vec2(hoveringCell));
    intVec2 endPos;
    float endPosDistance = -1;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            const glm::vec2 delta = glm::normalize(glm::vec2(x, y));
            const intVec2 cEndPos = startCell + intVec2(std::round(delta.x * startDistance), std::round(delta.y * startDistance));
            const float cEndPosDistance = glm::distance(glm::vec2(cEndPos), cursorPos/32.0f);
            if (endPosDistance == -1 || endPosDistance > cEndPosDistance) {
                endPosDistance = cEndPosDistance;
                endPos = cEndPos;
            }
        }
    }
    return endPos;
}

void ModifyCablingFeature::render() {
    this->visWiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);
}
