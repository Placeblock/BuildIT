//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERECEIVER_H
#define BUILDIT_NODERECEIVER_H

#include "glm/vec2.hpp"
#include "graphics/circuitBoard/elements/nodes/node.h"

class NodeReceiver {
public:
    virtual void receiveNode(glm::vec2 pos, std::unique_ptr<Node> node) = 0;
};

#endif //BUILDIT_NODERECEIVER_H
