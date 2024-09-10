//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NODERENDERER_H
#define BUILDIT_NODERENDERER_H


#include "graphics/programs.h"

#include "graphics/circuitBoard/components/nodes/node.h"

template<typename T>
class NodeRenderer : public Observer<MoveEvent> {
public:
    virtual void render(Programs *programs) = 0;
    virtual void addNode(T *node);
    virtual void removeNode(T *node);
    void notify(const MoveEvent &data) override = 0;
};


#endif //BUILDIT_NODERENDERER_H
