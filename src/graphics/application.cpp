//
// Created by felix on 8/7/24.
//

#include "application.h"
#include "graphics/font/fontLoader.h"
#include "graphics/gui/widgets/image.h"

void Application::onResize(intVec2 newSize) {
    this->size = newSize;

    this->guiView.root->updateSize(uintVec2(this->size.x, this->size.y));
}

void Application::onScroll(glm::vec2 offset) {
}

void Application::onKeyAction(int key, int scanCode, int action, int mods) {
}

void Application::onMouseAction(int button, int action, int mods) {
}

void Application::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
}

glm::vec2 Application::getMousePos() const {
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    return {x, y};
}

uintVec2 Application::getWindowSize() const {
    int x, y;
    glfwGetWindowSize(this->window, &x, &y);
    return intVec2{x, y};
}

void Application::render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    this->guiView.render(this->programs.textureProgram);
}

Application::Application(Sim::Simulation* simulation, GLFWwindow *window)
    : simulation(simulation), window(window), guiView(GUI::View{&this->programs}) {
	this->size = this->getWindowSize();

    std::unique_ptr<GUI::Element> kit = std::make_unique<Kit>(&this->guiView, this->simulation, &this->programs, this->getWindowSize());
    this->guiView.root = std::move(kit);
    this->guiView.regenerateBuffers();
}
