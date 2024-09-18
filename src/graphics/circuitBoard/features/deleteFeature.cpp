//
// Created by felix on 9/7/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "deleteFeature.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"
#include "graphics/circuitBoard/selection/selectable.h"

void DeleteFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        History::startBatch(this->history);
        for (const auto selectable: *this->selectionAccessor->getSelected()) {
            std::shared_ptr<Component> owningRef = this->compContainer->getOwningRef(selectable);
            std::unique_ptr<Action> dAction = std::make_unique<CreateComponentAction>(this->compContainer,
                                                                                      owningRef,
                                                                                      true);
            History::dispatch(this->history, dAction);
        }
        History::endBatch(this->history);
        this->selectionAccessor->clearSelection();
    }
}
