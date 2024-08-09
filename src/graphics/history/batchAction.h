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
    explicit BatchAction(bool reversed) : Action(reversed) {};
    void execute() override;
    void rewind() override;
    void addAction(const std::shared_ptr<Action>& action);
};


#endif //BUILDIT_BATCHACTION_H
