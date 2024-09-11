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
    explicit ComponentRenderers(FontRenderer *fontRenderer);

    CablingRenderer cablingRenderer;
    NotGateRenderer notGateRenderer;

    void render(Programs *programs);
};

class RendererAddVisitor : public Visitor {
private:
    ComponentRenderers *renderers;
public:
    explicit RendererAddVisitor(ComponentRenderers *renderers);

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
};

class RendererRemoveVisitor : public Visitor {
private:
    ComponentRenderers *renderers;
public:
    explicit RendererRemoveVisitor(ComponentRenderers *renderers);

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
};

class RendererMoveVisitor : public Visitor {
private:
    ComponentRenderers *renderers;
    glm::vec2 newPos;
public:
    RendererMoveVisitor(ComponentRenderers *renderers, glm::vec2 newPos);

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
};


#endif //BUILDIT_COMPONENTRENDERERS_H
