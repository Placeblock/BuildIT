//
// Created by felix on 8/4/24.
//

#include "meshManager.h"

void MeshManager::init() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(2, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    float gridVertices[] = {0, 0, 100, 100, 0, 100, 100, 0, 50, -100};
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    unsigned char colorData[] = {50, 150, 150, 150, 50, 150, 150, 150, 50};
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    this->counts = std::vector{3, 3, 3};
}

void MeshManager::render(Shader *shader) {
    int indices[3][3] = {{2, 1, 0}, {3, 0, 1}, {0, 3, 4}};
    shader->use();
    glBindVertexArray(this->vAO);
    glMultiDrawElements(GL_TRIANGLES, this->counts.data(), GL_UNSIGNED_INT, indices, 3);
}
