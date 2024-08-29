//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTJOINTACTION_H
#define BUILDIT_INSERTJOINTACTION_H


#include "graphics/circuitBoard/components/wires/wireContainer.h"
#include "graphics/circuitBoard/components/wires/jointContainer.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class InsertJointAction : public Action {
private:
    WireContainer *wireContainer;
    JointContainer *jointContainer;
    WiresRenderer *wiresRenderer;
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertJointAction(WireContainer *wireContainer, JointContainer *jointContainer, WiresRenderer *renderer,
                      const std::shared_ptr<Joint>& joint, bool reversed)
        : wireContainer(wireContainer), jointContainer(jointContainer), wiresRenderer(renderer), joint(joint), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_INSERTJOINTACTION_H
