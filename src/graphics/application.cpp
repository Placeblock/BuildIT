//
// Created by felix on 8/7/24.
//

#include "application.h"

void Application::onResize(int width, int height) {
    glViewport(0, 0, width, height);
    this->programs.gridProgram->setVec2("resolution", glm::vec2(width, height), false);
}

void Application::onScroll(double xOffset, double yOffset) {
    glm::vec2 mousePos = this->getMousePos();
    glm::vec2 worldMousePos = this->graphicData.camera.screenToWorld(mousePos);
    this->graphicData.camera.target = worldMousePos;
    this->graphicData.camera.offset = -mousePos;
    this->graphicData.camera.zoom+= 0.1f*float(yOffset)*this->graphicData.camera.zoom;
    this->programs.updateZoomUniforms();
}

void Application::onKeyAction(int key, int scanCode, int action, int mods) {

}

void Application::onMouseAction(int button, int action, int mods) {

}

void Application::onMouseMove(glm::vec2 abs, glm::vec2 delta) {

}

glm::vec2 Application::getMousePos() const {
    double x, y;
    glfwGetCursorPos(this->graphicData.window, &x, &y);
    return {x, y};
}

void Application::render() {

}
