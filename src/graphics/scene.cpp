//
// Created by felix on 8/10/24.
//

#include "scene.h"

#include "graphics/shapes/shapes.h"
#include "graphics/elements/nodes/gate.h"
#include "simulation/gate.h"
#include "image/stb_image.h"

void Scene::onResize(intVec2 newSize) {
    this->updateFrameBufferSize(newSize);
    this->world->onResize(this->calculateWorldSize());
    this->updateWorldQuadVertices();
}

intVec2 Scene::calculateWorldSize() {
    return this->size - intVec2(160, 0);
}

void Scene::render() {
    this->world->render();

    this->programs->updateProjectionUniforms(this->size, this->camera);

    this->useFrameBuffer();
    this->programs->textureProgram->use();
    glBindTexture(GL_TEXTURE_2D, this->world->texture);
    glBindVertexArray(this->worldVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    this->nodeList.render(this->programs->textureProgram);
    this->fontRenderer.render(this->programs->textureProgram);
}

void Scene::onScroll(glm::vec2 offset) {
    this->world->onScroll(offset);
}

void Scene::onKeyAction(int key, int scanCode, int keyAction, int mods) {
    this->world->onKeyAction(key, scanCode, keyAction, mods);
}

void Scene::onMouseAction(int button, int mouseAction, int mods) {
    if (this->mousePos.x < 160) {

    } else {
    }
    this->world->onMouseAction(button, mouseAction, mods);
}

void Scene::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
    this->mousePos = abs;
    if (abs.x > 160) {
        this->world->onMouseMove(abs - glm::vec2(160, 0), delta);
    }
}

Scene::Scene(Sim::Simulation* simulation, Programs* programs, Font newFont, intVec2 size)
    : simulation(simulation), programs(programs), fontRenderer(FontRenderer(newFont)), FrameBufferRenderable(size),
      nodeList(NodeList{&this->fontRenderer, nodeElements}){
    this->world = new World{programs, this->calculateWorldSize()};

    glGenVertexArrays(1, &this->worldVAO);
    glBindVertexArray(this->worldVAO);

    const std::vector<float> vertexData = this->generateWorldQuadVertices();

    glGenBuffers(3, this->worldVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*FULL_TEXTURE_COORDS.size(), FULL_TEXTURE_COORDS.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, FULL_TEXTURE_COLORS.size(), FULL_TEXTURE_COLORS.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(2);
}

void Scene::updateWorldQuadVertices() {
    const std::vector<float> vertexData = this->generateWorldQuadVertices();
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size()*sizeof(float), vertexData.data());
}

std::vector<float> Scene::generateWorldQuadVertices() {
    std::vector<float> vertices(12);
    vertices[0] = 160; vertices[1] = 0; vertices[2] = this->size.x;
    vertices[3] = 0; vertices[4] = this->size.x; vertices[5] = this->size.y;
    vertices[6] = this->size.x; vertices[7] = this->size.y; vertices[8] = 160;
    vertices[9] = this->size.y; vertices[10] = 160; vertices[11] = 0;
    return vertices;
}
