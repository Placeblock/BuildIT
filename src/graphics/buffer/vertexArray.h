//
// Created by felix on 8/30/24.
//

#ifndef BUILDIT_VERTEXARRAY_H
#define BUILDIT_VERTEXARRAY_H


#include "vertexBuffer.h"

class VertexArray {
private:
    unsigned int id;
    unsigned int attributes = 0;
public:
    VertexArray();
    template<typename T>
    void addBuffer(VertexBuffer<T> *buffer);
    void bind();
    void unbind();
    ~VertexArray();
};

template<typename T>
void VertexArray::addBuffer(VertexBuffer<T> *buffer) {
    this->bind();
    buffer->bind();
    const auto &elements = buffer->layout.getElements();
    unsigned int offset = 0;
    for (const auto &element: elements) {
        glVertexAttribPointer(this->attributes, element.count, element.type,
                              element.normalized ? GL_TRUE : GL_FALSE, elements.size() > 1 ? sizeof(T) : 0,
                              elements.size() > 1 ? (GLvoid*)offset : (void*)nullptr);
        glEnableVertexAttribArray(this->attributes);
        offset += element.getSize();
        this->attributes++;
    }
    this->unbind();
}


#endif //BUILDIT_VERTEXARRAY_H
