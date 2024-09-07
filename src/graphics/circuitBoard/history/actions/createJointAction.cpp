//
// Created by felix on 8/2/24.
//

#include "createJointAction.h"

void CreateJointAction::execute(bool lastInBatch) {
    if (this->createdNetwork == nullptr) { // Don't create new network if execute is used as redo
        this->createdNetwork = std::make_shared<Network>();
        this->joint->network = this->createdNetwork.get();
    }
    this->networkContainer->addNetwork(this->createdNetwork);
    this->jointContainer->addJoint(this->joint);

    this->wiresRenderer->regenerateJoints(this->jointContainer);
}

void CreateJointAction::rewind(bool lastInBatch) {
    if (this->createdNetwork == nullptr) {
        this->createdNetwork = this->networkContainer->getOwningRef(this->joint->network);
    }
    this->jointContainer->removeJoint(this->joint.get());
    this->networkContainer->removeNetwork(this->createdNetwork.get());

    this->wiresRenderer->regenerateJoints(this->jointContainer);
}

CreateJointAction::CreateJointAction(JointContainer* jointContainer, NetworkContainer *networkContainer,
                                     CablingRenderer *wiresRenderer, const std::shared_ptr<Joint> &joint, bool reversed)
        : jointContainer(jointContainer), networkContainer(networkContainer), wiresRenderer(wiresRenderer), joint(joint), Action(reversed) {
}