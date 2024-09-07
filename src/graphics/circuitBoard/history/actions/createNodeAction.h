//
// Created by felix on 8/22/24.
//

#ifndef BUILDIT_CREATENODEACTION_H
#define BUILDIT_CREATENODEACTION_H


#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"
#include "graphics/circuitBoard/components/nodes/nodeContainer.h"
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderer.h"

class CreateNodeAction : public Action {
private:
    NodeContainer *nodeContainer;
    NodeRenderer *renderer;
    std::shared_ptr<Node> node;
public:
    explicit CreateNodeAction(NodeContainer *nodeContainer, NodeRenderer *renderer, const std::shared_ptr<Node>& node, bool reversed)
    : nodeContainer(nodeContainer), node(node), renderer(renderer), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATENODEACTION_H
