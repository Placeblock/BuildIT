//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_HISTORY_H
#define BUILDIT_HISTORY_H


#include <stack>
#include <memory>
#include <deque>
#include "action.h"
#include "batchAction.h"
#include "graphics/circuitBoard/observer.h"

struct HistoryChangeEvent{};

class History final : public Subject<HistoryChangeEvent>{
    std::deque<std::unique_ptr<Action>> undoDeque;
    std::stack<std::unique_ptr<Action>> redoStack;
    std::unique_ptr<BatchAction> currentBatch;
    void addAction(std::unique_ptr<Action> action);
public:
    void startBatch();
    void dispatch(std::unique_ptr<Action>& action);
    void endBatch();
    void undo();
    void redo();
    static void startBatch(History *history);
    static void dispatch(History *history, std::unique_ptr<Action>& action);
    static void endBatch(History *history);
};


#endif //BUILDIT_HISTORY_H
