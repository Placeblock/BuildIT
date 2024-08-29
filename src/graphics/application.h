//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_APPLICATION_H
#define BUILDIT_APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "programs.h"
#include "graphics/data/renderer.h"
#include "graphics/kit/kit.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/gui/element.h"

class Application : public Renderer {
private:
    GLFWwindow* window;
    Programs programs{};
    Sim::Simulation* simulation;
    Camera camera{};
    intVec2 size; //TODO: uintVec2

    GUI::View guiView;
public:
    explicit Application(Sim::Simulation* simulation, GLFWwindow* window);
    void onResize(intVec2 newSize);
    void onScroll(glm::vec2 offset) const;
    void onKeyAction(int key, int scanCode, int action, int mods) const;
    void onMouseAction(int button, int action, int mods) const;
    void onMouseMove(glm::vec2 abs, glm::vec2 delta);
    void onChar(unsigned char c) const;

    [[nodiscard]] glm::vec2 getMousePos() const;
    [[nodiscard]] uintVec2 getWindowSize() const;

    void render() override;
};


#endif //BUILDIT_APPLICATION_H
