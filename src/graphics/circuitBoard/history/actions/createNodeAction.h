//
// Created by felix on 8/22/24.
//

#ifndef BUILDIT_CREATENODEACTION_H
#define BUILDIT_CREATENODEACTION_H


#include "graphics/circuitBoard/renderer/wiresRenderer.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/components/nodes/nodeContainer.h"

class CreateNodeAction : public Action {
private:
    NodeContainer *nodeContainer;
    std::shared_ptr<Node> node;
public:
    explicit CreateNodeAction(NodeContainer *nodeContainer, const std::shared_ptr<Node>& node, bool reversed)
    : nodeContainer(nodeContainer), node(node), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATENODEACTION_H
