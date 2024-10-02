//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deleteFeature.h"

#include "graphics/circuitBoard/components/cabling/joint.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/selection/selectable.h"
#include "graphics/circuitBoard/components/cabling/wire.h"

void DeleteFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        History::startBatch(this->history);
        for (const auto selectable: *this->selectionAccessor->getSelected()) {
            if (auto *joint = dynamic_cast<Joint*>(selectable)) {
                for (const auto &wire: joint->wires) {
                    if (this->selectionAccessor->isSelected(wire)) continue;
                    std::shared_ptr<Wire> owningRef = std::dynamic_pointer_cast<Wire>(this->compContainer->getOwningRef(wire));
                    std::unique_ptr<Action> dAction = std::make_unique<CreateComponentAction>(this->compContainer,
                                                                                              owningRef,
                                                                                              true);
                    History::dispatch(this->history, dAction);
                }
            }
            std::shared_ptr<Component> owningRef = this->compContainer->getOwningRef(selectable);
            if (owningRef == nullptr) continue;
            std::unique_ptr<Action> dAction = std::make_unique<CreateComponentAction>(this->compContainer,
                                                                                      owningRef,
                                                                                      true);
            History::dispatch(this->history, dAction);
        }
        History::endBatch(this->history);
        this->selectionAccessor->clearSelection();
    }
}
