//
// Created by felix on 8/30/24.
//

#include <GL/glew.h>
#include "vertexArray.h"

void VertexArray::bind() {
    glBindVertexArray(this->id);
}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &this->id);
    this->bind();
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &this->id);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}
