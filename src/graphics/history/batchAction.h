//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_BATCHACTION_H
#define BUILDIT_BATCHACTION_H


#include <vector>
#include <memory>
#include "action.h"

class BatchAction : public Action {
private:
    std::vector<std::shared_ptr<Action>> actions;
public:
    explicit BatchAction() : Action(false) {};
    void execute(bool isLastInBatch) override;
    void rewind(bool isLastInBatch) override;
    void addAction(const std::shared_ptr<Action>& action);
};


#endif //BUILDIT_BATCHACTION_H
