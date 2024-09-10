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
    this->renderers->notGateRenderer.removeNode(notGate);
}

void RendererRemoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.removeNetwork(joint->getNetwork());
}

RendererRemoveVisitor::RendererRemoveVisitor(ComponentRenderers *renderers) : renderers(renderers) {

}

void RendererMoveVisitor::doFor(NotGate *notGate) {
    this->renderers->notGateRenderer.updateInstance(notGate->getPos(), this->newPos, true);
}

void RendererMoveVisitor::doFor(Joint *joint) {
    this->renderers->cablingRenderer.notify({joint, this->newPos, false});
}

RendererMoveVisitor::RendererMoveVisitor(ComponentRenderers *renderers, glm::vec2 newPos)
    : renderers(renderers), newPos(newPos) {

}

ComponentRenderers::ComponentRenderers(FontRenderer *fontRenderer) : fontRenderer(fontRenderer), notGateRenderer(fontRenderer) {

}
