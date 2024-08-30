//
// Created by felix on 8/30/24.
//

#ifndef BUILDIT_VERTEXARRAY_H
#define BUILDIT_VERTEXARRAY_H


#include "vertexBuffer.h"

class VertexArray {
private:
    unsigned int id;
public:
    VertexArray();
    template<typename T>
    void addBuffer(VertexBuffer<T> *buffer);
    void bind();
    ~VertexArray();
};

template<typename T>
void VertexArray::addBuffer(VertexBuffer<T> *buffer) {
    this->bind();
    buffer->bind();
    const auto &elements = buffer->layout->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto &element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type,
                              element.normalized ? GL_TRUE : GL_FALSE, buffer->layout.getStride(),
                              (GLvoid*)offset);
        offset += element.getSize();
    }
}


#endif //BUILDIT_VERTEXARRAY_H
