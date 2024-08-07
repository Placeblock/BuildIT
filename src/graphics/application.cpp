//
// Created by felix on 8/7/24.
//

#include "application.h"

void Application::onResize(int width, int height) {
    glViewport(0, 0, width, height);
    this->mainScene->updateSize(glm::vec2(width, height));
}

void Application::onScroll(double xOffset, double yOffset) {
    glm::vec2 mousePos = this->getMousePos();

}

void Application::onKeyAction(int key, int scanCode, int action, int mods) {

}

void Application::onMouseAction(int button, int action, int mods) {

}

void Application::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
    this->mainScene->updateCursor(abs, delta);
}

glm::vec2 Application::getMousePos() const {
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    return {x, y};
}

void Application::render() {
    this->mainScene->render();
}

Application::Application(GLFWwindow *window) : window(window) {
    this->mainScene = new Scene(&this->programs);
    this->mainScene->use();
}
