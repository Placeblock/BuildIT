//
// Created by felix on 8/22/24.
//

#ifndef BUILDIT_NODEADDER_H
#define BUILDIT_NODEADDER_H

#include "glm/vec2.hpp"
#include "graphics/circuitBoard/circuitBoard.h"

class NodeAdder {
public:
    virtual std::unique_ptr<Node> addNode(CircuitBoard* board) = 0;
    virtual void removeNode() = 0;
    virtual void moveNode(glm::vec2 absPos) = 0;
};

#endif //BUILDIT_NODEADDER_H
