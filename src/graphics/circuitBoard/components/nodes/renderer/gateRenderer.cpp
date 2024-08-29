//
// Created by felix on 8/22/24.
//

#include "gateRenderer.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

void GateRenderer::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

void GateRenderer::addNode(Node* node) {
    this->addInstance(node->cell*32.0f);
    Gate *gate = static_cast<Gate*>(node);
    const std::string text = gate->text;
    glm::vec2 textPos = this->calcTextPos(node, node->cell);
    const std::shared_ptr<RenderedText> renderedText = this->fontRenderer->addText(text, Alignment::CENTER,
                                                                                   textPos, 30, {0, 255, 255});
    gate->renderedText = renderedText;
}

void GateRenderer::removeNode(Node* node) {
    this->removeInstance(node->cell*32.0f);
    Gate *gate = static_cast<Gate*>(node);
    this->fontRenderer->removeText(gate->renderedText);
    gate->renderedText.reset();
}

void GateRenderer::moveNode(Node *node, glm::vec2 newCell) {
    Gate *gate = static_cast<Gate*>(node);
    glm::vec2 textPos = this->calcTextPos(node, newCell);
    this->fontRenderer->moveText(gate->renderedText, textPos);
    InstancedMeshRenderer::updateInstance(node->cell*32.0f, newCell*32.0f, true);
}

glm::vec2 GateRenderer::calcTextPos(Node *node, glm::vec2 cell) {
    glm::vec2 textPos = cell*32.0f + glm::vec2(node->size*32)/2.0f;
    textPos.y -= 15.0f;
    return textPos;
}
