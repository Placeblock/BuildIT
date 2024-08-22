//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_MOVENODEACTION_H
#define BUILDIT_MOVENODEACTION_H


#include "graphics/circuitBoard/history/nodeAction.h"

class MoveNodeAction : public NodeAction {
private:
    intVec2 newPos;
    intVec2 oldPos;
    std::shared_ptr<Node> node;
public:
    explicit MoveNodeAction(Nodes* nodes, bool reversed, const std::shared_ptr<Node>& node, intVec2 newPos)
        : node(node), newPos(newPos), NodeAction(nodes, reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_MOVENODEACTION_H
