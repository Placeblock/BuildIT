//
// Created by felix on 8/4/24.
//

#include "instancedMeshRenderer.h"
#include "graphics/util.h"

BufferLayout getInstancedLayout() {
    return BufferLayout{BufferLayoutElement{GL_FLOAT, 2, false, 1}};
}

InstancedMeshRenderer::InstancedMeshRenderer(const std::vector<VertexData>& vertices, std::vector<unsigned int> indices)
    : vb(GL_ARRAY_BUFFER, Util::getDefaultLayout()), instancedBuffer(GL_ARRAY_BUFFER, getInstancedLayout()) {
    this->indexCount = indices.size();

    this->va.addBuffer(&this->vb);
    this->vb.bufferData(vertices);
    this->va.addBuffer(&this->instancedBuffer);
    this->va.bind();
    glGenVertexArrays(1, &this->indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    this->va.unbind();
}

void InstancedMeshRenderer::render(Program *shader) {
    if (!this->positions.empty()) {
        if (this->update && !this->rebuffer) {
            this->updateInstanceData();
        } else if (this->rebuffer) {
            this->rebufferInstanceData();
        }
        this->rebuffer = false;
        this->update = false;
        shader->use();
        this->va.bind();
        glDrawElementsInstanced(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, (void*)0, this->positions.size());
    }
}

void InstancedMeshRenderer::addInstance(glm::vec2 pos) {
    this->positions.push_back(pos);
    this->rebuffer = true;
}

void InstancedMeshRenderer::removeInstance(glm::vec2 pos) {
    this->positions.erase(std::remove(this->positions.begin(), this->positions.end(), pos), this->positions.end());
    this->rebuffer = true;
}

void InstancedMeshRenderer::updateInstance(glm::vec2 pos, glm::vec2 newPos) {
    if (pos == newPos) return;
    auto iter = std::find(this->positions.begin(), this->positions.end(), pos);
    *iter = newPos;
    this->update = true;
}

void InstancedMeshRenderer::updateInstanceData() {
    this->instancedBuffer.bind();
    this->instancedBuffer.bufferSubData(0, this->positions);
}

void InstancedMeshRenderer::rebufferInstanceData() {
    this->instancedBuffer.bufferData(this->positions);
}
