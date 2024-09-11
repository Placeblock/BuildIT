//
// Created by felix on 8/9/24.
//

#include "batchAction.h"
#include <utility>
#include "graphics/circuitBoard/history/action.h"

void BatchAction::execute(bool isLastInBatch) {
    for (int i = 0; i < this->actions.size(); i++) {
        Action::execute(this->actions[i].get(), i == this->actions.size() - 1);
    }
}

void BatchAction::rewind(bool isLastInBatch) {
    for (int i = this->actions.size() - 1; i >= 0; --i) {
        Action::rewind(this->actions[i].get(), i == 0);
    }
}

void BatchAction::addAction(std::unique_ptr<Action>& action) {
    this->actions.push_back(std::move(action));
}

size_t BatchAction::size() {
    return this->actions.size();
}
