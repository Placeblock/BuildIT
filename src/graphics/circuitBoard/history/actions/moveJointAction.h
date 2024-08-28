//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_MOVEJOINTACTION_H
#define BUILDIT_MOVEJOINTACTION_H


#include <utility>

#include "graphics/circuitBoard/history/wiresAction.h"
#include "simulation/simulation.h"
#include "graphics/circuitBoard/components/nodes/node.h"

class MoveJointAction : public Action {
private:
    JointContainer *jointContainer;
    WireContainer *wireContainer;
    WiresRenderer *wiresRenderer;
    std::shared_ptr<Joint> joint;
    glm::vec2 newCell;
    glm::vec2 oldCell;
    void updateCellData();
public:
    MoveJointAction(JointContainer *jointContainer, WireContainer *wireContainer,
                    WiresRenderer* renderer, const std::shared_ptr<Joint>& joint, glm::vec2 newCell)
        : jointContainer(jointContainer), wireContainer(wireContainer), wiresRenderer(renderer), joint(joint),
            newCell(newCell), Action(false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVEJOINTACTION_H
