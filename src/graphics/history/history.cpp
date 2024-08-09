//
// Created by felix on 8/9/24.
//

#include "history.h"

const long maxUndoDequeSize = 100;

void History::startBatch() {
    this->currentBatch = std::make_shared<BatchAction>(false);
}

void History::dispatch(const std::shared_ptr<Action>& action) {
    if (this->currentBatch != nullptr) {
        this->currentBatch->addAction(action);
    } else {
        this->addAction(action);
    }
}

void History::endBatch() {
    this->addAction(this->currentBatch);
    this->currentBatch = nullptr;
}

void History::undo() {
    this->redoStack.push(this->undoDeque.back());
    History::rewind(this->undoDeque.back()); // rewind
    this->undoDeque.pop_back();
}

void History::redo() {
    this->undoDeque.push_back(this->redoStack.top());
    History::execute(this->undoDeque.back()); // execute
    this->redoStack.pop();
}

void History::addAction(const std::shared_ptr<Action> &action) {
    this->undoDeque.push_back(action);
    if (this->undoDeque.size() > maxUndoDequeSize) {
        this->undoDeque.pop_front();
    }
}

void History::execute(const std::shared_ptr<Action> &action) {
    if (action->reversed) {
        action->rewind();
    } else {
        action->execute();
    }
}

void History::rewind(const std::shared_ptr<Action> &action) {
    if (action->reversed) {
        action->execute();
    } else {
        action->rewind();
    }
}
