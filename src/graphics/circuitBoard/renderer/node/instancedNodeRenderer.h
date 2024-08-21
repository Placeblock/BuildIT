//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_INSTANCEDNODERENDERER_H
#define BUILDIT_INSTANCEDNODERENDERER_H


#include <utility>

#include "nodeRenderer.h"
#include "graphics/renderer/instancedMeshRenderer.h"
#include "graphics/circuitBoard/elements/nodes/node.h"

template <typename N>
class InstancedNodeRenderer : public InstancedMeshRenderer, public NodeRenderer<N> {
public:
    InstancedNodeRenderer(std::vector<float> vertices, std::vector<unsigned char> colors, std::vector<unsigned int> indices)
        : NodeRenderer<N>(), InstancedMeshRenderer(std::move(vertices), std::move(colors), std::move(indices)) {};
    void render(Programs *programs) override;
    void addNode(N* node) override;
    void removeNode(N* node) override;
private:
    virtual std::vector<float> getVertices() = 0;
    virtual std::vector<unsigned char> getColors() = 0;
    virtual std::vector<unsigned int> getIndices() = 0;
};

template <typename N>
void InstancedNodeRenderer<N>::render(Programs *programs) {
    InstancedMeshRenderer::render(programs->instancedProgram);
}

template <typename N>
void InstancedNodeRenderer<N>::addNode(N* node) {
    this->addInstance(node->pos);
}

template <typename N>
void InstancedNodeRenderer<N>::removeNode(N* node) {
    this->removeInstance(node->pos);
}


#endif //BUILDIT_INSTANCEDNODERENDERER_H
