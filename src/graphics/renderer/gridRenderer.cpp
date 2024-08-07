//
// Created by felix on 8/2/24.
//

#include "gridRenderer.h"


GridRenderer::GridRenderer() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    unsigned int gridVBO;
    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    float gridVertices[] = {-1, -1, -1, 1, 1, -1,
                            -1, 1, 1, -1, 1, 1};
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
}

void GridRenderer::render(Program* shader) {
    shader->use();
    glBindVertexArray(this->vAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
