//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTJOINTACTION_H
#define BUILDIT_INSERTJOINTACTION_H


#include "graphics/circuitBoard/components/wires/wireContainer.h"
#include "graphics/circuitBoard/components/wires/jointContainer.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class InsertJointAction : public Action {
private:
    WireContainer *wireContainer;
    JointContainer *jointContainer;
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertJointAction(WireContainer *wireContainer, JointContainer *jointContainer,
                      const std::shared_ptr<Joint>& joint, const std::shared_ptr<Wire>& splitWire, bool reversed)
        : wireContainer(wireContainer), jointContainer(jointContainer), joint(joint), splitWire(splitWire), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_INSERTJOINTACTION_H
