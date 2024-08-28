//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEJOINTACTION_H
#define BUILDIT_CREATEJOINTACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"
#include "graphics/circuitBoard/components/nodes/node.h"

/**
 * Creates a single vertex with a new network
 */
class CreateJointAction : public Action {
private:
    JointContainer *jointContainer;
    NetworkContainer *networkContainer;
    WiresRenderer *wiresRenderer;
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Network> createdNetwork;
public:
    CreateJointAction(JointContainer *jointContainer, NetworkContainer *networkContainer, WiresRenderer *wiresRenderer, const std::shared_ptr<Joint> &joint, bool reversed);
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEJOINTACTION_H
