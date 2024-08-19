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

class Kit : public EventHandler, public FrameBufferRenderable {
public:
    Kit(Sim::Simulation* simulation, Programs* programs, Font font, intVec2 size);

    void render();

    void onResize(intVec2 newSize) override;
    void onScroll(glm::vec2 offset) override;
    void onKeyAction(int key, int scanCode, int keyAction, int mods) override;
    void onMouseAction(int button, int mouseAction, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;
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
