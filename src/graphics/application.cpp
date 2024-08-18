//
// Created by felix on 8/7/24.
//

#include "application.h"
#include "graphics/font/fontLoader.h"
#include "graphics/gui/widgets/image.h"
#include "graphics/gui/widgets/horizontalList.h"

void Application::onResize(intVec2 newSize) {
    this->size = newSize;
    this->updateSceneQuadVertices();
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
    //this->programs.textureProgram->use();
    //glBindTexture(GL_TEXTURE_2D, this->mainScene->texture);
    //glBindVertexArray(this->sceneVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    this->guiView.render(this->programs.textureProgram);
}

Application::Application(Sim::Simulation* simulation, GLFWwindow *window)
    : simulation(simulation), window(window) {
	this->size = this->getWindowSize();

    FontDataLoader fontDataLoader{"resources/font/data.fnt"};
    fontDataLoader.load();
    FontLoader fontLoader{fontDataLoader.fontData};
    fontLoader.load();

    this->mainScene = new Scene(this->simulation, &this->programs, fontLoader.font, this->getWindowSize());

    glGenVertexArrays(1, &this->sceneVAO);
    glBindVertexArray(this->sceneVAO);

    const std::vector<float> vertexData = this->generateSceneQuadVertices();

    glGenBuffers(3, this->sceneVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->sceneVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->sceneVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*FULL_TEXTURE_COORDS.size(), FULL_TEXTURE_COORDS.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->sceneVBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, FULL_TEXTURE_COLORS.size(), FULL_TEXTURE_COLORS.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(2);

    std::unique_ptr<GUI::Element> horList = std::make_unique<GUI::HorizontalList>(&this->guiView, uintVec2(this->size));
    std::unique_ptr<GUI::Element> sceneImage = std::make_unique<GUI::Image>(&this->guiView, this->size, this->mainScene->texture, horList.get());
    this->guiView.root = std::move(horList);
    this->guiView.root->addChild(sceneImage);
    this->guiView.regenerateBuffers();
}

void Application::updateSceneQuadVertices() {
    const std::vector<float> vertexData = this->generateSceneQuadVertices();
    glBindBuffer(GL_ARRAY_BUFFER, this->sceneVBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size()*sizeof(float), vertexData.data());
}

std::vector<float> Application::generateSceneQuadVertices() const {
    std::vector<float> vertices(12);
    vertices[0] = 0; vertices[1] = 0; vertices[2] = this->size.x;
    vertices[3] = 0; vertices[4] = this->size.x; vertices[5] = this->size.y;
    vertices[6] = this->size.x; vertices[7] = this->size.y; vertices[8] = 0;
    vertices[9] = this->size.y; vertices[10] = 0; vertices[11] = 0;
    return vertices;
}
