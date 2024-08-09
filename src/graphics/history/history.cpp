//
// Created by felix on 8/9/24.
//

#include "history.h"

const long maxUndoDequeSize = 100;

void History::startBatch() {
    this->currentBatch = std::make_shared<BatchAction>();
}

void History::dispatch(const std::shared_ptr<Action>& action) {
    while(!this->redoStack.empty()) this->redoStack.pop();
    if (this->currentBatch != nullptr) {
        this->currentBatch->addAction(action);
    } else {
        Action::execute(action, true);
        this->addAction(action);
    }
}

void History::endBatch() {
    while(!this->redoStack.empty()) this->redoStack.pop();
    this->addAction(this->currentBatch);
    Action::execute(this->currentBatch, true);
    this->currentBatch = nullptr;
}

void History::undo() {
    if (this->undoDeque.empty()) return;
    this->redoStack.push(this->undoDeque.back());
    Action::rewind(this->undoDeque.back(), true); // rewind
    this->undoDeque.pop_back();
}

void History::redo() {
    if (this->redoStack.empty()) return;
    this->undoDeque.push_back(this->redoStack.top());
    Action::execute(this->undoDeque.back(), true); // execute
    this->redoStack.pop();
}

void History::addAction(const std::shared_ptr<Action> &action) {
    this->undoDeque.push_back(action);
    if (this->undoDeque.size() > maxUndoDequeSize) {
        this->undoDeque.pop_front();
    }
}
