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

class RendererAddVisitor final : public Visitor {
    ComponentRenderers *renderers;
public:
    explicit RendererAddVisitor(ComponentRenderers *renderers);

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
    void doFor(Wire *wire) override;
};

class RendererRemoveVisitor final : public Visitor {
    ComponentRenderers *renderers;
public:
    explicit RendererRemoveVisitor(ComponentRenderers *renderers);

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
    void doFor(Wire *wire) override;
};

class RendererMoveVisitor final : public Visitor {
    ComponentRenderers *renderers;
    glm::vec2 delta;
public:
    RendererMoveVisitor(ComponentRenderers *renderers, glm::vec2 delta);

    void doFor(NotGate *notGate) override;
    void doFor(Joint *joint) override;
    void doFor(Wire *wire) override;
};


#endif //BUILDIT_COMPONENTRENDERERS_H
