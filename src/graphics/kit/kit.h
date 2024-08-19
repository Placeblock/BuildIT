//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_KIT_H
#define BUILDIT_KIT_H

#include "graphics/circuitBoard/circuitBoard.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/kit/nodeList/nodeList.h"


static const std::vector<NodeElementData> nodeElements = {
        NodeElementData{"Nicht", "gate_not.png"},
        NodeElementData{"Und", "gate_and.png"},
        NodeElementData{"Oder", "gate_or.png"},
};

class Kit : public FrameBufferRenderable, GUI::VerticalList {
public:
    Kit(Sim::Simulation* simulation, Programs* programs, Font font, intVec2 size);

    void render();
private:
    glm::vec2 mousePos;
    Programs* programs;
    Sim::Simulation* simulation;
    const Camera camera{}; // Default camera

    CircuitBoard* world;

    intVec2 calculateWorldSize();

    GLuint worldVAO;
    GLuint worldVBOs[3];
    std::vector<float> generateWorldQuadVertices();
    void updateWorldQuadVertices();

    FontRenderer fontRenderer;

    NodeList nodeList;

    void onDropNode(uint index);
    void onMouseDownNode(uint index);
};


#endif //BUILDIT_KIT_H
