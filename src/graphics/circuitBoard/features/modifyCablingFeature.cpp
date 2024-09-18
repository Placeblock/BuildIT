//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "modifyCablingFeature.h"
#include "graphics/circuitBoard/components/collisionDetection.h"
#include "graphics/circuitBoard/selection/selection.h"
#include "graphics/circuitBoard/history/history.h"
#include "graphics/circuitBoard/history/actions/createWireAction.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/history/actions/insertJointAction.h"

void ModifyCablingFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (mods & GLFW_MOD_CONTROL) return;
            glm::vec2 boardPos = this->cursorFeature->getHoveringCell() * 32;
            Component *colliding = this->collisionDetection->getColliding(boardPos);
            if ((colliding == nullptr || (dynamic_cast<Joint*>(colliding) && !(mods & GLFW_MOD_SHIFT)))
                    && (!(mods & GLFW_MOD_SHIFT) || this->selectionAccessor->getSelected()->empty())) {
                this->startCable(this->cursorFeature->getHoveringCell());
            }
        } else if (this->wire != nullptr && this->creating) {
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
    this->creating = false;
}

void ModifyCablingFeature::startCable(intVec2 cell) {
    this->startCell = cell;
    this->wire->start->move(cell * 32);
    intVec2 endCell = this->calculateEndCell();
    this->wire->end->move(endCell * 32);
    this->creating = true;
}

void ModifyCablingFeature::endCable() {
    this->creating = false;
    intVec2 endCell = this->calculateEndCell();
    if (endCell != this->startCell) {
        this->createCable(this->startCell, endCell);
    }
    this->aligned = true;
    this->endDelta = {};
}

void ModifyCablingFeature::createCable(intVec2 start, intVec2 end) {
    History::startBatch(this->history);
    Joint* pStartJoint = this->cabling->getJoint(start);
    Joint* pEndJoint = this->cabling->getJoint(end);
    if (pStartJoint == nullptr) {
        pStartJoint = this->createOrInsertJoint(start * 32);
    }
    if (pEndJoint == nullptr) {
        pEndJoint = this->createOrInsertJoint(end * 32);
    }
    std::shared_ptr<Wire> createdWire = std::make_shared<Wire>(pStartJoint, pEndJoint, pStartJoint->getNetwork());
    pStartJoint->getNetwork()->wires.push_back(createdWire.get());
    std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(this->wireContainer, createdWire, false);
    History::dispatch(this->history, dAction);
    History::endBatch(this->history);
}

Joint* ModifyCablingFeature::createOrInsertJoint(glm::vec2 pos) {
    std::unique_ptr<Action> dAction;
    Joint *jointRef;
    Wire *splitWire = this->cabling->getWire(pos);
    if (splitWire != nullptr) {
        std::unique_ptr<Joint> joint = std::make_unique<Joint>(pos, splitWire->getNetwork());
        jointRef = joint.get();
        std::shared_ptr<Wire> owningWire = this->wireContainer->getOwningRef(splitWire);
        dAction = std::make_unique<InsertJointAction>(this->wireContainer, this->componentContainer,
                                                      std::move(joint), owningWire, false);
    } else {
        std::shared_ptr<Network> network = std::make_shared<Network>();
        this->networkContainer->addNetwork(network);
        std::unique_ptr<Joint> joint = std::make_unique<Joint>(pos, network.get());
        jointRef = joint.get();
        network->joints.push_back(jointRef);
        dAction = std::make_unique<CreateComponentAction>(this->componentContainer, std::move(joint), false);
    }
    History::dispatch(this->history, dAction);
    return jointRef;
}

ModifyCablingFeature::ModifyCablingFeature(Programs *programs, History *history, CollisionDetection<Interactable> *cd,
                                           SelectionAccessor *selectionAccessor, CursorFeature *cursorFeature,
                                           WireContainer *wireContainer, ComponentContainer *componentContainer,
                                           Cabling *cabling, NetworkContainer *networkContainer)
    : history(history), collisionDetection(cd), selectionAccessor(selectionAccessor),
        cursorFeature(cursorFeature), wireContainer(wireContainer), componentContainer(componentContainer),
      Renderable(programs), cabling(cabling), networkContainer(networkContainer) {
    this->visNetwork->joints.push_back(this->startJoint.get());
    this->visNetwork->joints.push_back(this->endJoint.get());
    this->visNetwork->wires.push_back(this->wire.get());
    this->wire->connect();
    this->visWiresRenderer.addJoint(this->startJoint.get(), true);
    this->visWiresRenderer.addJoint(this->endJoint.get(), true);
    this->visWiresRenderer.addWire(this->wire.get(), true);
}

intVec2 ModifyCablingFeature::calculateEndCell() {
    intVec2 hoveringCell = this->cursorFeature->getHoveringCell();
    glm::vec2 cursorPos = this->cursorFeature->getCursorPos();
    if (!this->aligned) {
        return hoveringCell + this->endDelta;
    }
    const float startDistance = glm::distance(glm::vec2(this->startCell), glm::vec2(hoveringCell));
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
    return endPos + this->endDelta;
}

void ModifyCablingFeature::render() {
    if (this->creating) {
        this->visWiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);
    }
}

void ModifyCablingFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (!this->creating) return;
    if (key == GLFW_KEY_LEFT_SHIFT) {
        this->aligned = action != GLFW_PRESS;
    }
    if (action != GLFW_PRESS) return;
    if (key == GLFW_KEY_LEFT) {
        this->endDelta.x--;
    } else if (key == GLFW_KEY_RIGHT) {
        this->endDelta.x++;
    } else if (key == GLFW_KEY_UP) {
        this->endDelta.y--;
    } else if (key == GLFW_KEY_DOWN) {
        this->endDelta.y++;
    }
    intVec2 endCell = this->calculateEndCell();
    this->wire->end->move(endCell * 32);
}
