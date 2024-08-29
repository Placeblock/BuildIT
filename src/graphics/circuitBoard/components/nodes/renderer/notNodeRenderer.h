//
// Created by felix on 8/21/24.
//

#ifndef BUILDIT_NOTNODERENDERER_H
#define BUILDIT_NOTNODERENDERER_H

#include "graphics/circuitBoard/components/nodes/gate.h"
#include "graphics/shapes/shapes.h"

class NotNodeRenderer : public GateRenderer {
private:
    std::vector<float> getVertices() override {
        return Shapes::generateRoundedRectangle(96, 64, 5);
    };
    std::vector<unsigned char> getColors() override {
        return Shapes::getRepeatedColor(Color{255, 255, 0}, 29);
    };
    std::vector<unsigned int> getIndices() override{
        std::vector<unsigned int> notMeshIndices;
        Shapes::getRoundedRectangleIndices(notMeshIndices, 0);
        return notMeshIndices;
    };
public:
    NotNodeRenderer() : GateRenderer(this->getVertices(), this->getColors(), this->getIndices()) {}
};


#endif //BUILDIT_NOTNODERENDERER_H
