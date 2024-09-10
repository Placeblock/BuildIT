//
// Created by felix on 8/9/24.
//

#include "history.h"
#include <utility>
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/history/batchAction.h"

const long maxUndoDequeSize = 100;

void History::startBatch() {
    this->currentBatch = std::make_unique<BatchAction>();
}

void History::dispatch(std::unique_ptr<Action>& action) {
    while(!this->redoStack.empty()) this->redoStack.pop();
    if (this->currentBatch != nullptr) {
        this->currentBatch->addAction(action);
    } else {
        Action::execute(action.get(), true);
        this->addAction(std::move(action));
    }
}

void History::endBatch() {
    while(!this->redoStack.empty()) this->redoStack.pop();
    Action::execute(this->currentBatch.get(), true);
    this->addAction(std::move(this->currentBatch));
}

void History::undo() {
    if (this->undoDeque.empty()) return;
    this->notify({});
    Action::rewind(this->undoDeque.back().get(), true); // rewind
    this->redoStack.push(std::move(this->undoDeque.back()));
    this->undoDeque.pop_back();
}

void History::redo() {
    if (this->redoStack.empty()) return;
    this->notify({});
    Action::execute(this->redoStack.top().get(), true); // execute
    this->undoDeque.push_back(std::move(this->redoStack.top()));
    this->redoStack.pop();
}

void History::addAction(std::unique_ptr<Action> action) {
    this->undoDeque.push_back(std::move(action));
    if (this->undoDeque.size() > maxUndoDequeSize) {
        this->undoDeque.pop_front();
    }
}

void History::startBatch(History *history) {
    if (history != nullptr) history->startBatch();
}

void History::dispatch(History *history, std::unique_ptr<Action> &action) {
    if (history != nullptr) {
        history->dispatch(action);
    } else {
        Action::execute(action.get(), true);
    }
}

void History::endBatch(History *history) {
    if (history != nullptr) history->endBatch();
}
