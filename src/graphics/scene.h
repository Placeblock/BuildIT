//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_SCENE_H
#define BUILDIT_SCENE_H

#include "world.h"
#include "graphics/font/fontRenderer.h"
#include "nodeList.h"


static const std::vector<NodeElement> nodeElements = {
        NodeElement{"Nicht", "gate_not.png"},
        NodeElement{"Und", "gate_and.png"},
        NodeElement{"Oder", "gate_or.png"},
};

class Scene : public EventHandler, public FrameBufferRenderable {
public:
    Scene(Sim::Simulation* simulation, Programs* programs, Font font, intVec2 size);

    void render();

    void onResize(intVec2 newSize) override;
    void onScroll(glm::vec2 offset) override;
    void onKeyAction(int key, int scanCode, int keyAction, int mods) override;
    void onMouseAction(int button, int mouseAction, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;
private:
    Programs* programs;
    Sim::Simulation* simulation;
    const Camera camera{}; // Default camera

    World* world;

    intVec2 calculateWorldSize();

    GLuint worldVAO;
    GLuint worldVBOs[3];
    std::vector<float> generateWorldQuadVertices();
    void updateWorldQuadVertices();

    FontRenderer fontRenderer;

    NodeList nodeList;
};


#endif //BUILDIT_SCENE_H
