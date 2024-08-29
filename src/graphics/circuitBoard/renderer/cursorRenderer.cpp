//
// Created by felix on 8/2/24.
//

#include "cursorRenderer.h"
#include "graphics/data/program.h"

CursorRenderer::CursorRenderer() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(2, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    float gridVertices[] = {0, 0};
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    unsigned char colorData[] = {150, 150, 150};
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
}

void CursorRenderer::render(Program *shader) {
    shader->use();
    glBindVertexArray(this->vAO);
    glDrawArrays(GL_POINTS, 0, 1);
}

void CursorRenderer::update(glm::vec2 cursorPos) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    float cursorPosData[] = {cursorPos.x, cursorPos.y};
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(float), cursorPosData);
}
