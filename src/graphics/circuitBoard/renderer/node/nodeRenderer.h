//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"
#include "graphics/circuitBoard/elements/nodes/node.h"

template <typename N>
class NodeRenderer {
static_assert(std::is_base_of<Node, N>::value, "N must derive from Node");

public:
    virtual void render(Programs *programs) = 0;
    virtual void addNode(N* node) = 0;
    virtual void removeNode(N* node) = 0;
};


#endif //BUILDIT_NODERENDERER_H
