//
// Created by felix on 8/9/24.
//

#include "batchAction.h"

#include <ranges>

void BatchAction::execute() {
    for (const auto &item: this->actions) {
        item->execute();
    }
}

void BatchAction::rewind() {
    for (auto & action : std::ranges::reverse_view(this->actions)) {
        action->rewind();
    }
}

void BatchAction::addAction(const std::shared_ptr<Action>& action) {
    this->actions.push_back(action);
}
