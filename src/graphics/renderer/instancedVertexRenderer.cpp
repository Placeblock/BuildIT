//
// Created by felix on 8/6/24.
//

#include "instancedVertexRenderer.h"
#include "graphics/data/program.h"
#include "graphics/util.h"

void InstancedVertexRenderer::render(Program *shader) {
    if (this->count > 0) {
        shader->use();
        shader->setFloat("size", 10);
        this->va.bind();
        glDrawArraysInstanced(GL_POINTS, 0, 1, this->count);
    }
}

void InstancedVertexRenderer::updateVertices(std::vector<glm::vec2>* data) {
    this->count = data->size();
    this->instancedBuffer.bufferData(*data);
}

void InstancedVertexRenderer::updateVertex(unsigned int index, glm::vec2 newPos) {
    this->instancedBuffer.bind();
    this->instancedBuffer.bufferSubData(index, &newPos);
}

InstancedVertexRenderer::InstancedVertexRenderer() :
    instancedBuffer(GL_ARRAY_BUFFER, BufferLayout{BufferLayoutElement{GL_FLOAT, 2, false, 1}}) {
    VertexBuffer<glm::vec2> vertexBuffer{GL_ARRAY_BUFFER, Util::getDefaultVertexLayout()};
    vertexBuffer.bufferData(std::vector<glm::vec2>{glm::vec2{}});
    this->va.addBuffer(&vertexBuffer);
    this->va.addBuffer(&this->instancedBuffer);
}
