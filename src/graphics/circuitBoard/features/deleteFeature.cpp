//
// Created by felix on 9/7/24.
//

#include "deleteFeature.h"

void DeleteFeature::onKeyAction(glm::vec2 relPos, int key, int scanCode, int action, int mods) {
    if (!this->selection.joints.empty()) {
        this->history.startBatch();
        for (const auto &vertex: this->selection.joints) {
            auto wIter = vertex->wires.begin();
            while (wIter != vertex->wires.end()) {
                std::unique_ptr<Action> dAction = std::make_unique<CreateWireAction>(this->simulation, &this->wires,
                                                                                     &this->wires, &this->simBridge,
                                                                                     &this->wiresRenderer, this->wires.getOwningRef(*wIter++),
                                                                                     true);
                this->history.dispatch(dAction);
            }
            std::unique_ptr<Action> dAction = std::make_unique<CreateJointAction>(&this->simBridge, &this->wires, &this->wiresRenderer,
                                                                                  this->wires.getOwningRef(vertex), true);
            this->history.dispatch(dAction);
        }
        this->history.endBatch();
    }
    this->resetAction();
    this->selection.clear();
}
