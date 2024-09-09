//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTJOINTACTION_H
#define BUILDIT_INSERTJOINTACTION_H


#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/components/componentContainer.h"

class InsertJointAction : public Action {
private:
    WireContainer *wireContainer;
    ComponentContainer *compContainer;
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertJointAction(WireContainer *wireContainer, ComponentContainer *compContainer,
                      const std::shared_ptr<Joint>& joint, const std::shared_ptr<Wire>& splitWire, bool reversed)
        : wireContainer(wireContainer), compContainer(compContainer), joint(joint), splitWire(splitWire), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_INSERTJOINTACTION_H
