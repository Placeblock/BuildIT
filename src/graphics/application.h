//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_APPLICATION_H
#define BUILDIT_APPLICATION_H


#include "graphics/data/eventHandler.h"
#include "programs.h"
#include "graphics/data/renderer.h"
#include "types.h"
#include "scene.h"

class Application : public EventHandler, public Renderer {
private:
    GLFWwindow* window;
    Programs programs{};
    Scene* mainScene;
    Camera camera{};
    intVec2 size;

    GLuint sceneVAO;
    GLuint sceneVBOs[2];
public:
    explicit Application(GLFWwindow* window);
    void onResize(intVec2 newSize) override;
    void onScroll(glm::vec2 offset) override;
    void onKeyAction(int key, int scanCode, int action, int mods) override;
    void onMouseAction(int button, int action, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;

    [[nodiscard]] glm::vec2 getMousePos() const;
    [[nodiscard]] intVec2 getWindowSize() const;

    void render() override;
    std::vector<float> generateSceneQuadVertices();
    void updateSceneQuadVertices();
};


#endif //BUILDIT_APPLICATION_H
