//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_BATCHACTION_H
#define BUILDIT_BATCHACTION_H


#include <vector>
#include <memory>
#include "action.h"

class BatchAction final : public Action {
    std::vector<std::unique_ptr<Action>> actions;
public:
    explicit BatchAction() : Action(false) {};
    void execute(bool isLastInBatch) override;
    void rewind(bool isLastInBatch) override;
    void addAction(std::unique_ptr<Action>& action);
    [[nodiscard]] size_t size() const;
};


#endif //BUILDIT_BATCHACTION_H
