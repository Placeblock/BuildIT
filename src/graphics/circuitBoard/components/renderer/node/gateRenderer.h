//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_GATERENDERER_H
#define BUILDIT_GATERENDERER_H


#include "nodeRenderer.h"
#include "graphics/renderer/instancedMeshRenderer.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/circuitBoard/components/nodes/gate.h"

class GateRenderer : public InstancedMeshRenderer<Gate>, public NodeRenderer<Gate> {
public:
    GateRenderer(FontRenderer *fontRenderer, const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices)
        : InstancedMeshRenderer(vertices, indices), NodeRenderer(), fontRenderer(fontRenderer) {};
    void render(Programs *programs) override;
    void addNode(Gate* node) override;
    void removeNode(Gate* node) override;
    void notify(const MoveEvent &data) override;
private:
    FontRenderer *fontRenderer;
    std::unordered_map<Node*, std::unordered_set<std::unique_ptr<RenderedText>>> renderedTexts;
    virtual std::vector<VertexData> getData() = 0;
    virtual std::vector<unsigned int> getIndices() = 0;
    static glm::vec2 calcTextPos(const Node *node, glm::vec2 pos);
};

#endif //BUILDIT_GATERENDERER_H
