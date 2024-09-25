//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_CREATECOMPONENTACTION_H
#define BUILDIT_CREATECOMPONENTACTION_H


#include <memory>

#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/components/abstraction/component.h"
#include "graphics/circuitBoard/components/componentContainer.h"

class CreateComponentAction final : public Action {
    ComponentContainer *compContainer;
    std::shared_ptr<Component> component;
public:
    CreateComponentAction(ComponentContainer *compContainer, const std::shared_ptr<Component> &component, bool reversed);
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATECOMPONENTACTION_H
