//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_HISTORY_H
#define BUILDIT_HISTORY_H


#include <stack>
#include <memory>
#include "action.h"
#include "batchAction.h"

class History {
private:
    std::deque<std::shared_ptr<Action>> undoDeque;
    std::stack<std::shared_ptr<Action>> redoStack;
    std::shared_ptr<BatchAction> currentBatch;
    void addAction(const std::shared_ptr<Action>& action);
public:
    void startBatch();
    void dispatch(const std::shared_ptr<Action>& action);
    void endBatch();
    void undo();
    void redo();
};


#endif //BUILDIT_HISTORY_H
