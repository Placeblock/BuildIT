//
// Created by felix on 8/2/24.
//

#include <iostream>
#include "Lines.h"

void Lines::init() {
    glGenVertexArrays(2, this->vAOs);
    glGenBuffers(4, this->vBOs);

    glBindVertexArray(this->vAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->lines.size()*sizeof(float), this->lines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->lineColors.size() * sizeof(float), this->lineColors.data(), GL_STATIC_DRAW);
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

    Network network1;
    network1.vertices.push_back(Vertex(glm::vec2(0, 0), glm::vec3(0.87, 0.2, 0.16)));
    network1.vertices.push_back(Vertex(glm::vec2(320, 0), glm::vec3(0.87, 0.2, 0.16)));
    network1.vertices.push_back(Vertex(glm::vec2(320, 320), glm::vec3(0.87, 0.2, 0.16)));
    network1.lines.push_back(Line(&network1.vertices[0], &network1.vertices[1], glm::vec3(0.87, 0.2, 0.16)));
    network1.lines.push_back(Line(&network1.vertices[1], &network1.vertices[2], glm::vec3(0.87, 0.2, 0.16)));
    this->networks.push_back(network1);
    this->regenerateData();
}

void Lines::drawLines(Shader *shader) {
    shader->use();
    glBindVertexArray(this->vAOs[0]);
    glDrawArrays(GL_LINES, 0, this->lines.size()/2);
}

void Lines::drawVertices(Shader *shader) {
    shader->use();
    glBindVertexArray(this->vAOs[1]);
    glDrawArrays(GL_POINTS, 0, this->vertices.size()/2);
}

void Lines::draw(Shader *lineShader, Shader *vertexShader) {
    this->drawLines(lineShader);
    this->drawVertices(vertexShader);
}

void Lines::regenerateData() {
    this->vertices.clear();
    this->vertexColors.clear();
    this->lines.clear();
    this->lineColors.clear();
    for (const auto & network : this->networks) {
        network.fillVertices(&this->vertices);
        network.fillVertexColors(&this->vertexColors);
        network.fillLines(&this->lines);
        network.fillLineColors(&this->lineColors);
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->lines.size()*sizeof(float), this->lines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->lineColors.size() * sizeof(float), this->lineColors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColors.size() * sizeof(float), this->vertexColors.data(), GL_STATIC_DRAW);
}

void Network::fillVertices(std::vector<float> *array) const {
    for (const auto &vertex: this->vertices) {
        array->push_back(vertex.pos.x);
        array->push_back(vertex.pos.y);
    }
}

void Network::fillVertexColors(std::vector<float> *array) const {
    for (const auto &vertex: this->vertices) {
        array->push_back(vertex.color.x);
        array->push_back(vertex.color.y);
        array->push_back(vertex.color.z);
    }
}

void Network::fillLines(std::vector<float> *array) const {
    for (const auto &line: this->lines) {
        array->push_back(line.start->pos.x);
        array->push_back(line.start->pos.y);
        array->push_back(line.end->pos.x);
        array->push_back(line.end->pos.y);
    }
}

void Network::fillLineColors(std::vector<float> *array) const {
    for (const auto &line: this->lines) {
        array->push_back(line.color.x); // We need to do it twice (for each vertex)
        array->push_back(line.color.y);
        array->push_back(line.color.z);
        array->push_back(line.color.x);
        array->push_back(line.color.y);
        array->push_back(line.color.z);
    }
}

bool Network::isOnLine(glm::vec2 pos) {
    return false;
}
