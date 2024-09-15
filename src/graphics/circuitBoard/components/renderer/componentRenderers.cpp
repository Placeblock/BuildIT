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

RendererAddVisitor::RendererAddVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

void RendererRemoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.removeNode(notGate);
}

void RendererRemoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.removeJoint(joint, true);
}

RendererRemoveVisitor::RendererRemoveVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

void RendererMoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.notify({notGate, this->newPos, true});
}

void RendererMoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.notify({joint, this->newPos, true});
}

RendererMoveVisitor::RendererMoveVisitor(ComponentRenderers *renderers, glm::vec2 newPos)
    : renderers(renderers), newPos(newPos) {

}

ComponentRenderers::ComponentRenderers(FontRenderer *fontRenderer) : notGateRenderer(fontRenderer) {

}

void ComponentRenderers::render(Programs *programs) {
        this->notGateRenderer.render(programs);
        this->cablingRenderer.render(programs->wireProgram, programs->vertexProgram);
}
