//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_APPLICATION_H
#define BUILDIT_APPLICATION_H


#include "graphics/data/eventHandler.h"
#include "programs.h"
#include "graphics/data/renderer.h"
#include "scene.h"
#include "types.h"

class Application : public EventHandler, public Renderer {
private:
    GLFWwindow* window;
    Programs programs{};
    Scene* mainScene;
    Camera camera{};
    vpSize size;

    GLuint vAO;
    GLuint vBOs[2];
public:
    explicit Application(GLFWwindow* window);
    void onResize(int width, int height) override;
    void onScroll(double xOffset, double yOffset) override;
    void onKeyAction(int key, int scanCode, int action, int mods) override;
    void onMouseAction(int button, int action, int mods) override;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta) override;

    [[nodiscard]] glm::vec2 getMousePos() const;
    [[nodiscard]] vpSize getWindowSize() const;

    void render() override;
};


#endif //BUILDIT_APPLICATION_H
