//
// Created by felix on 9/10/24.
//

#include "componentRenderers.h"

void RendererAddVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.addNode(notGate);
}

void RendererAddVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.addNetwork(joint->getNetwork());
}

RendererAddVisitor::RendererAddVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

void RendererRemoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.removeNode(notGate, this->renderDelta);
}

void RendererRemoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.removeNetwork(joint->getNetwork());
}

RendererRemoveVisitor::RendererRemoveVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

RendererRemoveVisitor::RendererRemoveVisitor(ComponentRenderers *renderers, glm::vec2 renderDelta)
    : renderDelta(renderDelta), renderers(renderers) {

}

void RendererMoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.notify({notGate, this->newPos, false});
}

void RendererMoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.notify({joint, this->newPos, false});
}

RendererMoveVisitor::RendererMoveVisitor(ComponentRenderers *renderers, glm::vec2 newPos)
    : renderers(renderers), newPos(newPos) {

}

RendererMoveVisitor::RendererMoveVisitor(ComponentRenderers *renderers, glm::vec2 newPos, glm::vec2 oldPos)
    : renderers(renderers), newPos(newPos), oldPos(oldPos) {

}

ComponentRenderers::ComponentRenderers(FontRenderer *fontRenderer) : notGateRenderer(fontRenderer) {

}

void ComponentRenderers::render(Programs *programs) {
        this->notGateRenderer.render(programs);
        this->cablingRenderer.render(programs->wireProgram, programs->vertexProgram);
}
