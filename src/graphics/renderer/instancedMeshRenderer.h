//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_INSTANCEDMESHRENDERER_H
#define BUILDIT_INSTANCEDMESHRENDERER_H

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include "glm/gtx/hash.hpp"
#include <vector>
#include <unordered_set>
#include "graphics/data/program.h"
#include "graphics/buffer/vertexArray.h"
#include "graphics/util.h"

BufferLayout getInstancedLayout();

template<typename T>
class InstancedMeshRenderer {
public:
    InstancedMeshRenderer(const std::vector<VertexData>& vertices, std::vector<unsigned int> indices);
    void render(Program* shader);
    void addInstance(T* element, glm::vec2 pos);
    void removeInstance(T* element);
    void updateInstance(T* element, glm::vec2 newPos);
private:
    std::unordered_map<T*, Index*> indices;
    long indexCount;
    VertexArray va;
    VertexBuffer<VertexData> vb;
    IndexedBuffer<glm::vec2> instancedBuffer;
    GLuint indicesBuffer;
};

template<typename T>
InstancedMeshRenderer<T>::InstancedMeshRenderer(const std::vector<VertexData>& vertices, std::vector<unsigned int> indices)
        : vb(GL_ARRAY_BUFFER, Util::getDefaultLayout()), instancedBuffer(GL_ARRAY_BUFFER, getInstancedLayout()) {
    this->indexCount = indices.size();

    this->va.addBuffer(&this->vb);
    this->vb.bufferData(vertices);
    this->va.addBuffer(&this->instancedBuffer);
    this->va.bind();
    glGenBuffers(1, &this->indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    this->va.unbind();
}

template<typename T>
void InstancedMeshRenderer<T>::render(Program *shader) {
    if (this->instancedBuffer.size() != 0) {
        shader->use();
        this->va.bind();
        glDrawElementsInstanced(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, (void*)0, this->instancedBuffer.size());
    }
}

template<typename T>
void InstancedMeshRenderer<T>::addInstance(T* element, glm::vec2 pos) {
    this->indices[element] = this->instancedBuffer.addElement(pos);
    this->instancedBuffer.bufferAll();
}

template<typename T>
void InstancedMeshRenderer<T>::removeInstance(T* element) {
    this->instancedBuffer.removeElement(this->indices[element]);
    this->indices.erase(element);
    this->instancedBuffer.bufferAll();
}

template<typename T>
void InstancedMeshRenderer<T>::updateInstance(T* element, glm::vec2 newPos) {
    this->instancedBuffer.bind();
    this->instancedBuffer.updateElement(this->indices[element], newPos);
}



#endif //BUILDIT_INSTANCEDMESHRENDERER_H
