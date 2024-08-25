//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_MOVENODEACTION_H
#define BUILDIT_MOVENODEACTION_H


#include "graphics/circuitBoard/history/nodeAction.h"

class MoveNodeAction : public NodeAction {
private:
    glm::vec2 newCell;
    glm::vec2 oldCell;
    std::shared_ptr<Node> node;
public:
    explicit MoveNodeAction(Nodes* nodes, const std::shared_ptr<Node>& node, intVec2 newPos)
        : node(node), newCell(newPos), NodeAction(nodes, false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVENODEACTION_H
