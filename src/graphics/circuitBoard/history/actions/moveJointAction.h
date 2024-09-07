//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEJOINTACTION_H
#define BUILDIT_MOVEJOINTACTION_H

#include "graphics/circuitBoard/components/cabling/wireContainer.h"
#include "graphics/circuitBoard/components/cabling/jointContainer.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class MoveJointAction : public Action {
private:
    JointContainer *jointContainer;
    WireContainer *wireContainer;
    std::shared_ptr<Joint> joint;
    glm::vec2 newPos;
    glm::vec2 oldPos;
public:
    MoveJointAction(JointContainer *jointContainer, WireContainer *wireContainer,
                    const std::shared_ptr<Joint>& joint, glm::vec2 newPos)
        : jointContainer(jointContainer), wireContainer(wireContainer), joint(joint),
          newPos(newPos), Action(false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVEJOINTACTION_H
