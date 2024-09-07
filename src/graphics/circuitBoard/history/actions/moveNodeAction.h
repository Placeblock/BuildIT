//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_MOVENODEACTION_H
#define BUILDIT_MOVENODEACTION_H

#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/components/nodes/nodeContainer.h"

class MoveNodeAction : public Action {
private:
    glm::vec2 newPos;
    glm::vec2 oldPos;
    std::shared_ptr<Node> node;
public:
    explicit MoveNodeAction(const std::shared_ptr<Node>& node, intVec2 newPos)
        : node(node), newPos(newPos), Action(false) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVENODEACTION_H
