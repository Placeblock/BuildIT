//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"

class NodeRenderer;

#include "graphics/circuitBoard/components/nodes/node.h"

class NodeRenderer : public MultiObserver<MoveEvent<Node>, Node*> {
public:
    virtual void render(Programs *programs) = 0;
    virtual void addNode(Node *node);
    virtual void removeNode(Node *node);
    void update(const MoveEvent<Node> &data, Node *node) override = 0;
};


#endif //BUILDIT_NODERENDERER_H
