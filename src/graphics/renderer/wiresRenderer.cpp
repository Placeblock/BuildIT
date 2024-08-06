//
// Created by felix on 8/2/24.
//

#include "wiresRenderer.h"

void WiresRenderer::init() {
    glGenVertexArrays(2, this->vAOs);
    glGenBuffers(4, this->vBOs);

    glBindVertexArray(this->vAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->wireVertexData.size() * sizeof(float), this->wireVertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->wireColorData.size(), this->wireColorData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(this->vAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexData.size() * sizeof(float), this->vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColorData.size(), this->vertexColorData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
}

void WiresRenderer::drawWires(Program *shader) {
    if (this->wireVertexData.size() != 0) {
        shader->use();
        glBindVertexArray(this->vAOs[0]);
        glDrawArrays(GL_LINES, 0, this->wireVertexData.size() / 2);
    }
}

void WiresRenderer::drawVertices(Program *shader) {
    if (this->vertexData.size() != 0) {
        shader->use();
        glBindVertexArray(this->vAOs[1]);
        glDrawArrays(GL_POINTS, 0, this->vertexData.size() / 2);
    }
}

void WiresRenderer::render(Program *wireShader, Program *vertexShader) {
    this->drawWires(wireShader);
    this->drawVertices(vertexShader);
}

void WiresRenderer::fillVertices(std::set<std::shared_ptr<Vertex>>* vertices, std::vector<float> *vertexData, std::vector<unsigned char> *colorData) const {
    for (const auto &vertex: *vertices) {
        vertexData->push_back(vertex->cell.x * 32);
        vertexData->push_back(vertex->cell.y * 32);
        colorData->push_back(vertex->color.x);
        colorData->push_back(vertex->color.y);
        colorData->push_back(vertex->color.z);
    }
}

void WiresRenderer::fillWires(std::set<std::shared_ptr<Wire>>* wires, std::vector<float> *vertexData, std::vector<unsigned char> *colorData) const {
    for (const auto &wire: *wires) {
        vertexData->push_back(wire->start->cell.x * 32);
        vertexData->push_back(wire->start->cell.y * 32);
        vertexData->push_back(wire->end->cell.x * 32);
        vertexData->push_back(wire->end->cell.y * 32);
        colorData->push_back(wire->color.x);
        colorData->push_back(wire->color.y);
        colorData->push_back(wire->color.z);
        colorData->push_back(wire->color.x);
        colorData->push_back(wire->color.y);
        colorData->push_back(wire->color.z);
    }
}

void WiresRenderer::regenerateData(std::set<std::shared_ptr<Vertex>>* vertices, std::set<std::shared_ptr<Wire>>* wires) {
    this->vertexData.clear(); // OPTIMIZE MAYBE?
    this->vertexColorData.clear();
    this->wireVertexData.clear();
    this->wireColorData.clear();
    this->fillVertices(vertices, &this->vertexData, &this->vertexColorData);
    this->fillWires(wires, &this->wireVertexData, &this->wireColorData);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->wireVertexData.size() * sizeof(float), this->wireVertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->wireColorData.size(), this->wireColorData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexData.size() * sizeof(float), this->vertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColorData.size(), this->vertexColorData.data(), GL_STATIC_DRAW);
}

void WiresRenderer::updateVertexPos(int index, glm::vec2 newPos) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    float newPosData[2] = {newPos.x*32, newPos.y*32};
    glBufferSubData(GL_ARRAY_BUFFER, 2*sizeof(float)*index, 2*sizeof(float), newPosData);
}

void WiresRenderer::updateVertexColor(int index, glm::vec3 newColor) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    unsigned char newColorData[3];
    newColorData[0] = newColor.x;
    newColorData[1] = newColor.y;
    newColorData[2] = newColor.z;
    glBufferSubData(GL_ARRAY_BUFFER, 3*index, 3, newColorData);
}

void WiresRenderer::updateWirePos(int index, glm::vec2 start, glm::vec2 end) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    float newPos[4] = {start.x*32, start.y*32, end.x*32, end.y*32};
    glBufferSubData(GL_ARRAY_BUFFER, 4*sizeof(float)*index, 4*sizeof(float), newPos);
}

void WiresRenderer::updateWireColor(int index, glm::vec3 newColor) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    unsigned char newColorData[6];
    newColorData[0] = newColor.x;
    newColorData[1] = newColor.y;
    newColorData[2] = newColor.z;
    newColorData[3] = newColor.x;
    newColorData[4] = newColor.y;
    newColorData[5] = newColor.z;
    glBufferSubData(GL_ARRAY_BUFFER, 6*index, 6, newColorData);
}
