//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_APPLICATION_H
#define BUILDIT_APPLICATION_H


#include "graphics/data/eventHandler.h"
#include "programs.h"
#include "graphics/data/renderer.h"
#include "scene.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/gui/widget.h"

class Application : public EventHandler, public Renderer {
private:
    GLFWwindow* window;
    Programs programs{};
    Sim::Simulation* simulation;
    Scene* mainScene;
    Camera camera{};
    intVec2 size; //TODO: uintVec2

    GLuint sceneVAO;
    GLuint sceneVBOs[3];

    GUI::View guiView;
public:
    explicit Application(Sim::Simulation* simulation, GLFWwindow* window);
    void onResize(intVec2 newSize) override;
    void onScroll(glm::vec2 offset) override;
    void onKeyAction(int key, int scanCode, int action, int mods) override;
    void onMouseAction(int button, int action, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;

    [[nodiscard]] glm::vec2 getMousePos() const;
    [[nodiscard]] intVec2 getWindowSize() const;

    void render() override;
    [[nodiscard]] std::vector<float> generateSceneQuadVertices() const;
    void updateSceneQuadVertices();
};


#endif //BUILDIT_APPLICATION_H
