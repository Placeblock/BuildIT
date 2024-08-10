//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_SCENE_H
#define BUILDIT_SCENE_H


#include "world.h"

class Scene : public EventHandler, public FrameBufferRenderable {
public:
    Scene(Programs* programs, intVec2 size);

    void render();

    void onResize(intVec2 newSize) override;
    void onScroll(glm::vec2 offset) override;
    void onKeyAction(int key, int scanCode, int keyAction, int mods) override;
    void onMouseAction(int button, int mouseAction, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;
private:
    Programs* programs;
    const Camera camera{}; // Default camera

    World* world;

    intVec2 calculateWorldSize();

    GLuint worldVAO;
    GLuint worldVBOs[2];
    std::vector<float> generateWorldQuadVertices();
    void updateWorldQuadVertices();
};


#endif //BUILDIT_SCENE_H
