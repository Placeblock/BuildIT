//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODECONTAINER_H
#define BUILDIT_NODECONTAINER_H

#include <memory>
#include "glm/glm.hpp"
#include "node.h"
#include "nodeEvents.h"

class NodeContainer : public Subject<NodeAddEvent>, public Subject<NodeRemoveEvent> {
private:
    std::list<std::shared_ptr<Node>> nodes;
public:
    void addNode(const std::shared_ptr<Node>& node);
    void removeNode(Node* node);
};

#endif //BUILDIT_NODECONTAINER_H
