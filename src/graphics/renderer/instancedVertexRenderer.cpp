//
// Created by felix on 8/6/24.
//

#include "instancedVertexRenderer.h"
#include "graphics/data/program.h"

void InstancedVertexRenderer::init() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(2, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    const float vertices[] = {0};
    glBufferData(GL_ARRAY_BUFFER, sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[1]);
}

void InstancedVertexRenderer::render(Program *shader) {
    if (this->count > 0) {
        shader->use();
        shader->setFloat("size", 10);
        glBindVertexArray(this->vAO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->vBOs[1]);
        glDrawArraysInstanced(GL_POINTS, 0, GL_UNSIGNED_INT, this->count);
    }
}

void InstancedVertexRenderer::updateVertices(std::vector<glm::vec2>* data) {
    this->count = data->size();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->count*sizeof(glm::vec2), data->data(), GL_DYNAMIC_DRAW);
}

void InstancedVertexRenderer::updateVertex(int index, glm::vec2 newPos) {
    glm::vec2 newPosData[1] = {newPos};
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[1]);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, index*sizeof(newPos), sizeof(newPos), newPosData);
}
