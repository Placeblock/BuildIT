//
// Created by felix on 8/22/24.
//

#include "gateRenderer.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

void GateRenderer::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

void GateRenderer::addNode(Gate* gate) {
    this->NodeRenderer<Gate>::addNode(gate);
    this->addInstance(gate->getPos());
    const std::string text = gate->text;
    glm::vec2 textPos = GateRenderer::calcTextPos(gate, gate->getPos());
    std::unique_ptr<RenderedText> renderedText = this->fontRenderer->addText(text, Alignment::CENTER,
                                                                                   textPos, 30, Color{0, 255, 255});
    this->renderedTexts[gate].insert(std::move(renderedText));
}

void GateRenderer::removeNode(Gate* gate) {
    this->NodeRenderer<Gate>::removeNode(gate);
    this->removeInstance(gate->getPos());
    for (const auto &renderedText: this->renderedTexts[gate]) {
    	this->fontRenderer->removeText(renderedText.get());
    }
    this->renderedTexts.erase(gate);
}

void GateRenderer::removeNode(Gate *gate, glm::vec2 renderDelta) {
    this->removeInstance(gate->getPos() + renderDelta);
    for (const auto &renderedText: this->renderedTexts[gate]) {
        this->fontRenderer->removeText(renderedText.get());
    }
    this->renderedTexts.erase(gate);
}


void GateRenderer::notify(const MoveEvent &data) {
    if (!data.before) return;
    if (Node *node = dynamic_cast<Node*>(data.movable)) {
        glm::vec2 textPos = GateRenderer::calcTextPos(node, data.newPos);
        for (const auto &renderedText: this->renderedTexts[node]) {
            this->fontRenderer->moveText(renderedText.get(), textPos);
        }
        InstancedMeshRenderer::updateInstance(node->getPos(), data.newPos);
    }
}

glm::vec2 GateRenderer::calcTextPos(Node *node, glm::vec2 pos) {
    glm::vec2 textPos = pos + glm::vec2(node->cellSize * 32) / 2.0f;
    textPos.y -= 15.0f;
    return textPos;
}