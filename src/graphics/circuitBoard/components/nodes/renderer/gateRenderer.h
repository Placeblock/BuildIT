//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_GATERENDERER_H
#define BUILDIT_GATERENDERER_H


#include <utility>

#include "nodeRenderer.h"
#include "graphics/renderer/instancedMeshRenderer.h"
#include "graphics/font/fontRenderer.h"

class GateRenderer : public InstancedMeshRenderer, public NodeRenderer {
public:
    GateRenderer(FontRenderer *fontRenderer, std::vector<float> vertices,
                 std::vector<unsigned char> colors, std::vector<unsigned int> indices)
        : fontRenderer(fontRenderer), NodeRenderer(), InstancedMeshRenderer(std::move(vertices),
                                                                            std::move(colors), std::move(indices)) {};
    void render(Programs *programs) override;
    void addNode(Node* node) override;
    void removeNode(Node* node) override;
    void notify(Subject<MoveEvent> *subject, const MoveEvent &data) override;
private:
    FontRenderer *fontRenderer;
    std::unordered_map<Node*, std::unordered_set<std::unique_ptr<RenderedText>>> renderedTexts;
    virtual std::vector<float> getVertices() = 0;
    virtual std::vector<unsigned char> getColors() = 0;
    virtual std::vector<unsigned int> getIndices() = 0;
    static glm::vec2 calcTextPos(Node *node, glm::vec2 pos);
};

#endif //BUILDIT_GATERENDERER_H
