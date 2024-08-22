//
// Created by felix on 8/22/24.
//

#include "instancedNodeRenderer.h"

void InstancedNodeRenderer::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

void InstancedNodeRenderer::addNode(Node* node) {
    this->addInstance(node->pos);
}

void InstancedNodeRenderer::removeNode(Node* node) {
    this->removeInstance(node->pos);
}
