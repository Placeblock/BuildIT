//
// Created by felix on 8/7/24.
//

#include "application.h"

void Application::onResize(intVec2 newSize) {
    this->size = newSize;
    this->mainScene->onResize(this->size);
}

void Application::onScroll(glm::vec2 offset) {
	this->mainScene->onScroll(offset);
}

void Application::onKeyAction(int key, int scanCode, int action, int mods) {
    this->mainScene->onKeyAction(key, scanCode, action, mods);
}

void Application::onMouseAction(int button, int action, int mods) {
	this->mainScene->onMouseAction(button, action, mods);
}

void Application::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
    glm::vec2 mousePos = abs;
    this->mainScene->onMouseMove(abs, delta);
}

glm::vec2 Application::getMousePos() const {
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    return {x, y};
}

intVec2 Application::getWindowSize() const {
    int x, y;
    glfwGetWindowSize(this->window, &x, &y);
    return intVec2{x, y};
}

void Application::render() {
    this->mainScene->render();
    this->programs.updateProjectionUniforms(this->size, this->camera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, this->size.x, this->size.y);
    this->programs.textureProgram->use();
    glBindTexture(GL_TEXTURE_2D, this->mainScene->texture);
    glBindVertexArray(this->vAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

Application::Application(GLFWwindow *window) : window(window) {
	this->size = this->getWindowSize();
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
