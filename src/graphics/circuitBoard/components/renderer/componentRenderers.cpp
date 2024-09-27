//
// Created by felix on 9/10/24.
//

#include "componentRenderers.h"

void RendererAddVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.addNode(notGate);
}

void RendererAddVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.addJoint(joint, true);
}

void RendererAddVisitor::doFor(Wire *wire) {
    this->renderers->cablingRenderer.addWire(wire, true);
}

RendererAddVisitor::RendererAddVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

void RendererRemoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.removeNode(notGate);
}

void RendererRemoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.removeJoint(joint, true);
}

void RendererRemoveVisitor::doFor(Wire *wire) {
    this->renderers->cablingRenderer.removeWire(wire, true);
}

RendererRemoveVisitor::RendererRemoveVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

void RendererMoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.notify({notGate, this->delta, true});
}

void RendererMoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.notify({joint, this->delta, true});
}

void RendererMoveVisitor::doFor(Wire *wire) {
    this->renderers->cablingRenderer.notify({wire, this->delta, true});
}

RendererMoveVisitor::RendererMoveVisitor(ComponentRenderers *renderers, const glm::vec2 delta)
    : renderers(renderers), delta(delta) {

}

ComponentRenderers::ComponentRenderers(FontRenderer *fontRenderer) : notGateRenderer(fontRenderer) {

}

void ComponentRenderers::render(Programs *programs) {
        this->notGateRenderer.render(programs);
        this->cablingRenderer.render(programs->wireProgram, programs->vertexProgram);
}
