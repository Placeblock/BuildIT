//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_MOVENODEACTION_H
#define BUILDIT_MOVENODEACTION_H


#include "graphics/circuitBoard/history/nodeAction.h"
#include "graphics/circuitBoard/components/nodes/nodeContainer.h"

class MoveNodeAction : public Action {
private:
    NodeContainer *nodeContainer;
    glm::vec2 newCell;
    glm::vec2 oldCell;
    std::shared_ptr<Node> node;
public:
    explicit MoveNodeAction(NodeContainer *nodeContainer, const std::shared_ptr<Node>& node, intVec2 newPos)
        : nodeContainer(nodeContainer), node(node), newCell(newPos), Action(false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVENODEACTION_H
