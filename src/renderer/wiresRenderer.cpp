//
// Created by felix on 8/2/24.
//

#include "wiresRenderer.h"

void WiresRenderer::init() {
    glGenVertexArrays(2, this->vAOs);
    glGenBuffers(4, this->vBOs);

    glBindVertexArray(this->vAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->wireVertices.size() * sizeof(float), this->wireVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->wireColors.size() * sizeof(float), this->wireColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(this->vAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColors.size() * sizeof(float), this->vertexColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    this->regenerateData();
}

void WiresRenderer::drawWires(Shader *shader) {
    shader->use();
    glBindVertexArray(this->vAOs[0]);
    glDrawArrays(GL_LINES, 0, this->wireVertices.size() / 2);
}

void WiresRenderer::drawVertices(Shader *shader) {
    shader->use();
    glBindVertexArray(this->vAOs[1]);
    glDrawArrays(GL_POINTS, 0, this->vertices.size()/2);
}

void WiresRenderer::draw(Shader *wireShader, Shader *vertexShader) {
    this->drawWires(wireShader);
    this->drawVertices(vertexShader);
}

void WiresRenderer::regenerateData() {
    this->vertices.clear();
    this->vertexColors.clear();
    this->wireVertices.clear();
    this->wireColors.clear();
    this->wires->fillVertices(&this->vertices, &this->vertexColors);
    this->wires->fillWires(&this->wireVertices, &this->wireColors);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->wireVertices.size() * sizeof(float), this->wireVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->wireColors.size() * sizeof(float), this->wireColors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColors.size() * sizeof(float), this->vertexColors.data(), GL_STATIC_DRAW);
}

WiresRenderer::WiresRenderer(Wires *wires) {
    this->wires = wires;
}

void WiresRenderer::updateVertexPos(std::shared_ptr<Vertex> vertex) {
    long index = std::distance(this->wires->vertexMap.begin(), this->wires->vertexMap.find(vertex));
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    float newPos[2] = {vertex->cell.x*32, vertex->cell.y*32};
    glBufferSubData(GL_ARRAY_BUFFER, 2*sizeof(float)*index, 2*sizeof(float), newPos);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    for (const auto &wire: vertex->wires) {
        index = std::distance(this->wires->wireMap.begin(), this->wires->wireMap.find(wire));
        float newPos[4] = {wire->start->cell.x*32, wire->start->cell.y*32, wire->end->cell.x*32, wire->end->cell.y*32};
        glBufferSubData(GL_ARRAY_BUFFER, 4*sizeof(float)*index, 4*sizeof(float), newPos);
    }
}
