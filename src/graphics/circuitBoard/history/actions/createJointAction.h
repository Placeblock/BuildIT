//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CREATEJOINTACTION_H
#define BUILDIT_CREATEJOINTACTION_H

#include "graphics/circuitBoard/components/cabling/jointContainer.h"
#include "graphics/circuitBoard/components/cabling/networkContainer.h"
#include "graphics/circuitBoard/history/action.h"

/**
 * Creates a single vertex with a new network
 */
class CreateJointAction : public Action {
private:
    JointContainer *jointContainer;
    NetworkContainer *networkContainer;
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Network> createdNetwork;
public:
    CreateJointAction(JointContainer *jointContainer, NetworkContainer *networkContainer, const std::shared_ptr<Joint> &joint, bool reversed);
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEJOINTACTION_H
