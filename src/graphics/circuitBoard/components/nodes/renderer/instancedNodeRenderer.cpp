//
// Created by felix on 8/22/24.
//

#include "instancedNodeRenderer.h"

void GateRenderer::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

void GateRenderer::addNode(Node* node) {
    this->addInstance(node->cell*32.0f);
}

void GateRenderer::removeNode(Node* node) {
    this->removeInstance(node->cell*32.0f);
}

void GateRenderer::moveNode(Node *node, glm::vec2 newCell) {
    InstancedMeshRenderer::updateInstance(node->cell*32.0f, newCell*32.0f, true);
}
