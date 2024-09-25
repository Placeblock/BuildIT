//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTJOINTACTION_H
#define BUILDIT_INSERTJOINTACTION_H


#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/components/componentContainer.h"

class InsertJointAction final : public Action {
    ComponentContainer *compContainer;
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertJointAction(ComponentContainer *compContainer,
                      const std::shared_ptr<Joint>& joint, const std::shared_ptr<Wire>& splitWire, bool reversed)
        : compContainer(compContainer), joint(joint), splitWire(splitWire), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_INSERTJOINTACTION_H
