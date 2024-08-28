//
// Created by felix on 8/28/24.
//

#ifndef BUILDIT_NODECONTAINER_H
#define BUILDIT_NODECONTAINER_H

#include <memory>
#include "glm/glm.hpp"
#include "node.h"

class NodeContainer {
public:
    virtual void moveNode(Node* node, glm::vec2 newPos, bool updateSSBO) = 0;
    virtual void addNode(const std::shared_ptr<Node>& node) = 0;
    virtual void removeNode(Node* node) = 0;
};

#endif //BUILDIT_NODECONTAINER_H
