//
// Created by felix on 8/4/24.
//

#include "meshManager.h"

void MeshManager::init() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(2, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    float gridVertices[] = {0, 0, 100, 100, 0, 100, 100, 0};
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    unsigned char colorData[] = {50, 150, 150};
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);

    this->indices = std::vector{2, 1, 0, 3};
}

void MeshManager::render(Shader *shader) {
    shader->use();
    glBindVertexArray(this->vAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, this->indices.data());
}
