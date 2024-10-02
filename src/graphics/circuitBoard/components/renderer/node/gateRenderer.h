//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_GATERENDERER_H
#define BUILDIT_GATERENDERER_H


#include "nodeRenderer.h"
#include "graphics/renderer/instancedMeshRenderer.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

template<typename T>
class GateRenderer : public InstancedMeshRenderer<Gate>, public NodeRenderer<T> {
public:
    GateRenderer(FontRenderer *fontRenderer, const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices)
        : InstancedMeshRenderer(vertices, indices), NodeRenderer<T>(), fontRenderer(fontRenderer) {};
    void render(Programs *programs) override;
    void addComponent(T* gate) override;
    void removeComponent(T* gate) override;
    void notify(const MoveEvent &data) override;
private:
    FontRenderer *fontRenderer;
    std::unordered_map<Gate*, std::unordered_set<std::unique_ptr<RenderedText>>> renderedTexts;
    virtual std::vector<VertexData> getData() = 0;
    virtual std::vector<unsigned int> getIndices() = 0;
    static glm::vec2 calcTextPos(const Gate *node, glm::vec2 pos);
};


template<typename T>
void GateRenderer<T>::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

template<typename T>
void GateRenderer<T>::addComponent(T* gate) {
    std::cout << "ADDING COMPONENT2\n";
    this->NodeRenderer<T>::addComponent(gate);
    this->addInstance(gate, gate->getPos());
    const std::string text = gate->text;
    const glm::vec2 textPos = calcTextPos(gate, gate->getPos());
    std::unique_ptr<RenderedText> renderedText = this->fontRenderer->addText(text, Alignment::CENTER,
                                                                             textPos, 30, Color{0, 255, 255, 255});
    this->renderedTexts[gate].insert(std::move(renderedText));
}

template<typename T>
void GateRenderer<T>::removeComponent(T* gate) {
    this->NodeRenderer<T>::removeComponent(gate);
    this->removeInstance(gate);
    for (const auto &renderedText: this->renderedTexts[gate]) {
        this->fontRenderer->removeText(renderedText.get());
    }
    this->renderedTexts.erase(gate);
}


template<typename T>
void GateRenderer<T>::notify(const MoveEvent &data) {
    if (!data.before) return;
    if (const auto gate = dynamic_cast<Gate*>(data.movable)) {
        const glm::vec2 newPos = gate->getPos() + data.delta;
        const glm::vec2 textPos = calcTextPos(gate, newPos);
        for (const auto &renderedText: this->renderedTexts[gate]) {
            this->fontRenderer->moveText(renderedText.get(), textPos);
        }
        updateInstance(gate, newPos);
    }
}

template<typename T>
glm::vec2 GateRenderer<T>::calcTextPos(const Gate *node, const glm::vec2 pos) {
    glm::vec2 textPos = pos + glm::vec2(node->cellSize * 32) / 2.0f;
    textPos.y -= 15.0f;
    return textPos;
}

#endif //BUILDIT_GATERENDERER_H
