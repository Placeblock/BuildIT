//
// Created by felix on 9/7/24.
//

#include "deleteFeature.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/history/actions/createComponentAction.h"

void DeleteFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    History::startBatch(this->history);
    for (const auto &component: *this->selectionFeature->getComponents()) {
        std::shared_ptr<Component> owningRef = this->compContainer->getOwningRef(component);
        std::unique_ptr<Action> dAction = std::make_unique<CreateComponentAction>(this->compContainer,
                                                                                  owningRef,
                                                                             true);
        History::dispatch(this->history, dAction);
    }
    History::endBatch(this->history);
    this->history->endBatch();
    this->selectionFeature->clearSelection();

}
