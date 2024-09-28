//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NOTGATERENDERER_H
#define BUILDIT_NOTGATERENDERER_H

#include "graphics/circuitBoard/components/nodes/notGate.h"
#include "graphics/shapes/shapes.h"
#include "gateRenderer.h"

class NotGateRenderer : public GateRenderer<NotGate> {
    std::vector<VertexData> getData() override {
        return Shapes::generateRoundedRectangle(96, 64, 5, Color{255, 255, 0, 255});
    };
    std::vector<unsigned int> getIndices() override{
        std::vector<unsigned int> notMeshIndices;
        Shapes::getRoundedRectangleIndices(notMeshIndices, 0);
        return notMeshIndices;
    };
public:
    explicit NotGateRenderer(FontRenderer *fontRenderer)
            : GateRenderer(fontRenderer, this->NotGateRenderer::getData(),
                           this->NotGateRenderer::getIndices()) {}
};


#endif //BUILDIT_NOTGATERENDERER_H
