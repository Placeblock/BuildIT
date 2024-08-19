//
// Created by felix on 8/9/24.
//

#include "history.h"

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
    Action::rewind(this->undoDeque.back().get(), true); // rewind
    this->redoStack.push(std::move(this->undoDeque.back()));
    this->undoDeque.pop_back();
}

void History::redo() {
    if (this->redoStack.empty()) return;
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
