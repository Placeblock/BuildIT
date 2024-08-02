//
// Created by felix on 8/2/24.
//

#include "cursorRenderer.h"

void CursorRenderer::init() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    unsigned int cursorVBO;
    glGenBuffers(1, &cursorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
    float gridVertices[] = {0};
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
}

void CursorRenderer::draw(Shader *shader) {
    shader->use();
    glBindVertexArray(this->vAO);
    glDrawArrays(GL_POINTS, 0, 1);
}
