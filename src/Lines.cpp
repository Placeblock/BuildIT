//
// Created by felix on 8/2/24.
//

#include <iostream>
#include "Lines.h"

void Lines::init() {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(3, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColors.size() * sizeof(float), this->vertexColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColors.size() * sizeof(float), this->vertexColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(2);

    this->lines.emplace_back(glm::vec3(0, 0.55, 0), std::vector<Vertex>{
            Vertex(glm::vec2(0, 0), glm::vec3(0.87, 0.2, 0.16)),
            Vertex(glm::vec2(320, 0), glm::vec3(0.87, 0.2, 0.16)),
            Vertex(glm::vec2(320, 320), glm::vec3(0.87, 0.2, 0.16)),
    });
    this->lines.emplace_back(glm::vec3(0.87, 0.2, 0.16), std::vector<Vertex>{
            Vertex{glm::vec2(0, 96), glm::vec3(0.87, 0.2, 0.16)},
            Vertex{glm::vec2(96, 192), glm::vec3(0.87, 0.2, 0.16)},
            Vertex{glm::vec2(256, 192), glm::vec3(0.87, 0.2, 0.16)},
    });
    this->regenerateData();
}

void Lines::drawLines(Shader *shader) {
    shader->use();
    glMultiDrawArrays(GL_LINE_STRIP, this->indices.data(), this->sizes.data(), this->lines.size());
}

void Lines::drawVertices(Shader *shader) {
    shader->use();
    glDrawArrays(GL_POINTS, 0, this->vertices.size()/2);
}

void Lines::draw(Shader *lineShader, Shader *vertexShader) {
    glBindVertexArray(this->vAO);
    this->drawLines(lineShader);
    this->drawVertices(vertexShader);
}

void Lines::regenerateData() {
    this->vertices.clear();
    this->vertexColors.clear();
    this->indices.clear();
    this->sizes.clear();
    int offset = 0;
    for (const auto & line : this->lines) {
        line.fillVertices(&this->vertices);
        line.fillVertexColors(&this->vertexColors);
        line.fillLineColor(&this->lineColors);
        this->indices.push_back(offset);
        this->sizes.push_back(line.vertices.size());
        offset += line.vertices.size();
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->vertexColors.size() * sizeof(float), this->vertexColors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->lineColors.size() * sizeof(float), this->lineColors.data(), GL_STATIC_DRAW);
}

void Line::fillVertices(std::vector<float>* array) const {
    for (const auto & vertex : this->vertices) {
        array->push_back(vertex.pos.x);
        array->push_back(vertex.pos.y);
    }
}

void Line::fillVertexColors(std::vector<float>* array) const {
    for (const auto & vertex : this->vertices) {
        array->push_back(vertex.color.x);
        array->push_back(vertex.color.y);
        array->push_back(vertex.color.z);
    }
}

void Line::fillLineColor(std::vector<float>* array) const {
    for (int i = 0; i < this->vertices.size(); ++i) {
        array->push_back(this->color.x);
        array->push_back(this->color.y);
        array->push_back(this->color.z);
    }
}

void Line::setVertexColor(int i, glm::vec3 color) {
    this->vertices[i].color = color;
}

void Line::setVertexColor(glm::vec2 pos, glm::vec3 color) {
    for (auto &vertex: this->vertices) {
        if (vertex.pos == pos) {
            vertex.color = color;
        }
    }
}
