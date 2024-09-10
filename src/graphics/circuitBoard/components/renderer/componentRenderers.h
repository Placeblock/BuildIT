//
// Created by felix on 9/10/24.
//

#ifndef BUILDIT_COMPONENTRENDERERS_H
#define BUILDIT_COMPONENTRENDERERS_H


#include "cablingRenderer.h"
#include "graphics/circuitBoard/components/renderer/node/notGateRenderer.h"
#include "graphics/circuitBoard/components/nodes/notGate.h"

class ComponentRenderers {
public:
    CablingRenderer cablingRenderer;
    NotGateRenderer notGateRenderer;

};

class RendererAddVisitor : public Visitor {
private:
    ComponentRenderers *renderers;
public:
    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
};

class RendererRemoveVisitor : public Visitor {
private:
    ComponentRenderers *renderers;
public:
    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
};


#endif //BUILDIT_COMPONENTRENDERERS_H
