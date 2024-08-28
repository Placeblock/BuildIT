//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"

class NodeRenderer;

#include "graphics/circuitBoard/components/nodes/node.h"

class NodeRenderer {
public:
    virtual void render(Programs *programs) = 0;
    virtual void addNode(Node* node) = 0;
    virtual void removeNode(Node* node) = 0;
    virtual void moveNode(Node* node, glm::vec2 newCell) = 0;
};


#endif //BUILDIT_NODERENDERER_H
