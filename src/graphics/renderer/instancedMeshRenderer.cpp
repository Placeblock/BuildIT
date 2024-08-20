//
// Created by felix on 8/4/24.
//

#include "instancedMeshRenderer.h"


InstancedMeshRenderer::InstancedMeshRenderer(std::vector<float> vertices, std::vector<unsigned char> colors,
                                             std::vector<unsigned int> indices) {
    this->indexCount = indices.size();
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(4, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    //float gridVertices[] = {0, 0, 100, 100, 0, 100, 100, 0, 50, -100};
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    //unsigned char colorData[] = {50, 150, 150, 150, 50, 150, 150, 150, 50};
    glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(unsigned char), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[3]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->vBOs[3]);
}

void InstancedMeshRenderer::render(Program *shader) {
    if (this->positions.size() > 0) {
        shader->use();
        glBindVertexArray(this->vAO);
        glDrawElementsInstanced(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, (void*)0, this->positions.size());
    }
}

void InstancedMeshRenderer::addInstance(glm::vec2 pos) {
    this->positions.push_back(pos);
    this->updateSSBO();
}

void InstancedMeshRenderer::removeInstance(glm::vec2 pos) {
    this->positions.erase(std::remove(this->positions.begin(), this->positions.end(), pos), this->positions.end());
    this->updateSSBO();
}

void InstancedMeshRenderer::updateInstance(glm::vec2 pos, glm::vec2 newPos, bool updateSSBO) {
    const auto iter = std::find(this->positions.begin(), this->positions.end(), pos);
    long index = std::distance(this->positions.begin(), iter);
    this->positions[index] = newPos;
    if (updateSSBO) {
        glm::vec2 newPosData[1] = {newPos};
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[3]);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, index, sizeof(newPosData), newPosData);
    }
}

void InstancedMeshRenderer::updateInstance(int index, glm::vec2 newPos, bool updateSSBO) {
    this->positions[index] = newPos;
    if (updateSSBO) {
        glm::vec2 newPosData[1] = {newPos};
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[3]);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, index, sizeof(newPosData), newPosData);
    }
}


void InstancedMeshRenderer::updateSSBO() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->vBOs[3]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->positions.size()*sizeof(glm::vec2), this->positions.data(), GL_DYNAMIC_DRAW);
}