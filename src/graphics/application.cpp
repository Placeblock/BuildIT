//
// Created by felix on 8/7/24.
//

#include "application.h"

void Application::onResize(int width, int height) {
    this->size = vpSize(width, height);
    this->mainScene->updateSize(this->size);
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

vpSize Application::getWindowSize() const {
    int x, y;
    glfwGetWindowSize(this->window, &x, &y);
    return vpSize{x, y};
}

void Application::render() {
    this->mainScene->render();
    this->programs.updateProjectionUniforms(this->size, this->camera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->size.x, this->size.y);
    this->programs.textureProgram->use();
    glBindVertexArray(this->vAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

Application::Application(GLFWwindow *window) : window(window) {
    this->mainScene = new Scene(&this->programs, this->getWindowSize());
    this->mainScene->use();

    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    static const GLfloat vertexData[] = {
            -1, 1, 1, 1, 1, -1,
            1, -1, -1, -1, -1, 1
    };
    static const GLfloat textureData[] = {
            0, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 1
    };

    glGenBuffers(2, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
}
