//
// Created by felix on 8/22/24.
//

#include "instancedNodeRenderer.h"

void InstancedNodeRenderer::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

void InstancedNodeRenderer::addNode(Node* node) {
    this->addInstance(node->cell*32.0f);
}

void InstancedNodeRenderer::removeNode(Node* node) {
    this->removeInstance(node->cell*32.0f);
}

void InstancedNodeRenderer::moveNode(Node *node, glm::vec2 newCell) {
    InstancedMeshRenderer::updateInstance(node->cell*32.0f, newCell*32.0f, true);
}
