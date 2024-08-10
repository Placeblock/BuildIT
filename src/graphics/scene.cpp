//
// Created by felix on 8/10/24.
//

#include "scene.h"

void Scene::onResize(intVec2 newSize) {
    this->updateFrameBufferSize(newSize);
    this->world->onResize(this->calculateWorldSize());
    this->updateWorldQuadVertices();
}

intVec2 Scene::calculateWorldSize() {
    return this->size - intVec2(200, 0);
}

void Scene::render() {
    this->world->render();

    this->programs->updateProjectionUniforms(this->size, this->camera);

    this->useFrameBuffer();
    this->programs->textureProgram->use();
    glBindTexture(GL_TEXTURE_2D, this->world->texture);
    glBindVertexArray(this->worldVAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void Scene::onScroll(glm::vec2 offset) {

}

void Scene::onKeyAction(int key, int scanCode, int keyAction, int mods) {

}

void Scene::onMouseAction(int button, int mouseAction, int mods) {

}

void Scene::onMouseMove(glm::vec2 abs, glm::vec2 delta) {

}

Scene::Scene(Programs* programs, intVec2 size)
    : programs(programs), FrameBufferRenderable(size) {
    this->world = new World{programs, this->calculateWorldSize()};

    glGenVertexArrays(1, &this->worldVAO);
    glBindVertexArray(this->worldVAO);

    const std::vector<float> vertexData = this->generateWorldQuadVertices();
    static const GLfloat textureData[] = {
            0, 1, 1, 1, 1, 0,
            1, 0, 0, 0, 0, 1
    };

    glGenBuffers(2, this->worldVBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
}

void Scene::updateWorldQuadVertices() {
    const std::vector<float> vertexData = this->generateWorldQuadVertices();
    glBindBuffer(GL_ARRAY_BUFFER, this->worldVBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size()*sizeof(float), vertexData.data());
}

std::vector<float> Scene::generateWorldQuadVertices() {
    std::vector<float> vertices(12);
    vertices[0] = 200; vertices[1] = 0; vertices[2] = this->size.x;
    vertices[3] = 0; vertices[4] = this->size.x; vertices[5] = this->size.y;
    vertices[6] = this->size.x; vertices[7] = this->size.y; vertices[8] = 200;
    vertices[9] = this->size.y; vertices[10] = 200; vertices[11] = 0;
    return vertices;
}
