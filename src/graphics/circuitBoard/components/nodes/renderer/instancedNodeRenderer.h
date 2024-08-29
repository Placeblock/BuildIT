//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_INSTANCEDNODERENDERER_H
#define BUILDIT_INSTANCEDNODERENDERER_H


#include <utility>

#include "nodeRenderer.h"
#include "graphics/renderer/instancedMeshRenderer.h"

class GateRenderer : public InstancedMeshRenderer, public NodeRenderer {
public:
    GateRenderer(std::vector<float> vertices, std::vector<unsigned char> colors, std::vector<unsigned int> indices)
        : NodeRenderer(), InstancedMeshRenderer(std::move(vertices), std::move(colors), std::move(indices)) {};
    void render(Programs *programs) override;
    void addNode(Node* node) override;
    void removeNode(Node* node) override;
    void moveNode(Node* node, glm::vec2 newCell) override;
private:
    virtual std::vector<float> getVertices() = 0;
    virtual std::vector<unsigned char> getColors() = 0;
    virtual std::vector<unsigned int> getIndices() = 0;
};

#endif //BUILDIT_INSTANCEDNODERENDERER_H
