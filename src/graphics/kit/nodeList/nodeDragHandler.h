//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODEDRAGHANDLER_H
#define BUILDIT_NODEDRAGHANDLER_H

#include "glm/vec2.hpp"
#include "graphics/circuitBoard/elements/nodes/node.h"
#include "nodeAdder.h"

class NodeDragHandler {
public:
    virtual void setActiveNodeAdder(NodeAdder *adder) = 0;
    virtual float getBoardZoom() = 0;
};

#endif //BUILDIT_NODEDRAGHANDLER_H
