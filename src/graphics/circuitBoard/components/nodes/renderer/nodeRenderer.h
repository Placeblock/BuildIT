//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"

class NodeRenderer;

#include "graphics/circuitBoard/components/nodes/node.h"

class NodeRenderer : public Observer<MoveEvent> {
public:
    virtual void render(Programs *programs) = 0;
    virtual void addNode(Node *node);
    virtual void removeNode(Node *node);
    void notify(Subject<MoveEvent> *subject, const MoveEvent &data) override = 0;
};


#endif //BUILDIT_NODERENDERER_H
