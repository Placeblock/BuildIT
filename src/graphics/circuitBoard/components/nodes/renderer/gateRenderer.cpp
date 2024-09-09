//
// Created by felix on 8/22/24.
//

#include "gateRenderer.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

void GateRenderer::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

void GateRenderer::addNode(Node* node) {
    this->addInstance(node->getPos());
    Gate *gate = static_cast<Gate*>(node);
    const std::string text = gate->text;
    glm::vec2 textPos = this->calcTextPos(node, node->getPos());
    std::unique_ptr<RenderedText> renderedText = this->fontRenderer->addText(text, Alignment::CENTER,
                                                                                   textPos, 30, Color{0, 255, 255});
    this->renderedTexts[node].insert(std::move(renderedText));
}

void GateRenderer::removeNode(Node* node) {
    this->removeInstance(node->getPos());
    for (const auto &renderedText: this->renderedTexts[node]) {
    	this->fontRenderer->removeText(renderedText.get());
    }
    this->renderedTexts.erase(node);
}

void GateRenderer::notify(Subject<MoveEvent> *subject, const MoveEvent &data) {
    Node *node = dynamic_cast<Node*>(subject);
    glm::vec2 textPos = GateRenderer::calcTextPos(node, data.newPos);
    for (const auto &renderedText: this->renderedTexts[node]) {
    	this->fontRenderer->moveText(renderedText.get(), textPos);
    }
    InstancedMeshRenderer::updateInstance(node->getPos(), data.newPos, true);
}

glm::vec2 GateRenderer::calcTextPos(Node *node, glm::vec2 pos) {
    glm::vec2 textPos = pos + glm::vec2(node->size) / 2.0f;
    textPos.y -= 15.0f;
    return textPos;
}
