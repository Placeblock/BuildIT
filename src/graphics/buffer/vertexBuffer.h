//
// Created by felix on 8/30/24.
//

#ifndef BUILDIT_VERTEXBUFFER_H
#define BUILDIT_VERTEXBUFFER_H


#include <cstddef>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

struct BufferSection {
    size_t index;
    size_t elementIndex;
    unsigned int elements;
};

struct BufferLayoutElement {
    unsigned int type;
    unsigned int count;
    bool normalized;
public:
    [[nodiscard]] unsigned int getSize() const;
    static unsigned int getTypeSize(unsigned int type);
};

struct BufferLayout {
private:
    std::vector<BufferLayoutElement> elements;
public:
    void addElement(BufferLayoutElement element);
    std::vector<BufferLayoutElement> getElements();
};

struct RenderedElement {
    size_t index;
};

template<typename T>
class VertexBuffer {
public:
    VertexBuffer(unsigned int type, BufferLayout  layout);
    VertexBuffer(unsigned int type, BufferLayout  layout, const std::vector<T> &data);
    unsigned int type;
    unsigned int id = 0;
    BufferLayout layout;
    void bind();
    void bufferData(const std::vector<T>& data);
    void bufferSubData(unsigned int offset, const T& data);
    void bufferSubData(unsigned int offset, const std::vector<T>& data);

    virtual ~VertexBuffer();
};
template<typename T>
VertexBuffer<T>::VertexBuffer(unsigned int type, BufferLayout layout, const std::vector<T> &data)
    : layout(std::move(layout)), type(type) {
    glGenBuffers(1, &this->id);
    glBindBuffer(this->type, this->id);
    glBufferData(this->type, sizeof(T) * data.size(), data.data());
}

template<typename T>
VertexBuffer<T>::VertexBuffer(unsigned int type, BufferLayout layout) : layout(std::move(layout)), type(type) {
    glGenBuffers(1, &this->id);
    glBindBuffer(this->type, this->id);
}

template<typename T>
VertexBuffer<T>::~VertexBuffer() {
    glDeleteBuffers(1, &this->id);
}

template<typename T>
void VertexBuffer<T>::bind() {
    glBindBuffer(this->type, this->id);
}

template<typename T>
void VertexBuffer<T>::bufferData(const std::vector<T> &data) {
    this->bind();
    glBufferData(this->type, sizeof(T) * data.size(), data.data(), GL_DYNAMIC_DRAW);
}

template<typename T>
void VertexBuffer<T>::bufferSubData(unsigned int offset, const T &data) {
    glBufferSubData(this->type, sizeof(T)*offset, sizeof(T), data);
}

template<typename T>
void VertexBuffer<T>::bufferSubData(unsigned int offset, const std::vector<T> &data) {
    glBufferSubData(this->type, sizeof(T)*offset, sizeof(T) * data.size(), data.data());
}

template<typename T>
class IndexedBuffer : public VertexBuffer<T> {
protected:
    std::vector<T> data;
    std::vector<std::unique_ptr<RenderedElement>> elements;
public:
    IndexedBuffer(unsigned int type, BufferLayout layout) : VertexBuffer<T>(type, layout) {};
    virtual RenderedElement* addElement(T data);
    virtual void removeElement(RenderedElement *element);
    size_t getSize();
};

template<typename T>
size_t IndexedBuffer<T>::getSize() {
    return this->elements.size();
}

template<typename T>
inline void IndexedBuffer<T>::removeElement(RenderedElement *element) {
    this->data.erase(this->data.begin() + element->index);
    this->elements.erase(this->elements.begin() + element->index);
}

template<typename T>
inline RenderedElement *IndexedBuffer<T>::addElement(T data) {
    this->elements.push_back(std::make_unique<RenderedElement>(this->data.size()));
    this->data.push_back(data);
    return this->elements.back().get();
}

template<typename T>
class SectionedBuffer : public VertexBuffer<T> {
    std::vector<T> data;
    std::vector<std::unique_ptr<BufferSection>> sections;
public:
    SectionedBuffer(unsigned int type, BufferLayout layout) : VertexBuffer<T>(type, layout) {};
    BufferSection* addElement(T newData);
    BufferSection* addElement(const std::vector<T> & newData);
    void addElement(T data, BufferSection *section);
    void removeSection(BufferSection *section);
    void bufferSection(BufferSection *section, const std::vector<T> &newData);
    void bufferAll();
    size_t getSize();
};

template<typename T>
size_t SectionedBuffer<T>::getSize() {
    return this->data.size();
}

template<typename T>
void SectionedBuffer<T>::bufferAll() {
    this->bufferData(this->data);
}

template<typename T>
void SectionedBuffer<T>::bufferSection(BufferSection *section, const std::vector<T> &newData) {
    assert((("Tried to write " + std::to_string(newData.size()) +
            " elements to section which contains " + std::to_string(section->elements) + " elements").c_str(), newData.size() == section->elements));
    glBufferSubData(this->type, sizeof(T)*section->elementIndex, sizeof(T) * newData.size(), newData.data());
}

template<typename T>
inline void SectionedBuffer<T>::addElement(T data, BufferSection *section) {
    section->elements++;
    size_t index = section->index + section->elements;
    this->data.insert(this->data.begin() + index, data);
    auto sIter = this->sections.begin() + section->index;
    while (++sIter != this->sections.end()) {
        (*sIter)->elementIndex++;
    }
}

template<typename T>
inline BufferSection* SectionedBuffer<T>::addElement(T newData) {
    this->sections.push_back(std::make_unique<BufferSection>(this->sections.size(), this->data.size(), 1));
    this->data.push_back(newData);
    return this->sections.back().get();
}

template<typename T>
BufferSection *SectionedBuffer<T>::addElement(const std::vector<T> &newData) {
    this->sections.push_back(std::make_unique<BufferSection>(this->sections.size(), this->data.size(), newData.size()));
    this->data.insert(this->data.end(), newData.begin(), newData.end());
    return this->sections.back().get();
}

template<typename T>
void SectionedBuffer<T>::removeSection(BufferSection *section) {
    const auto dataStart = this->data.begin() + section->elementIndex;
    const auto dataEnd = this->data.begin() + section->elementIndex + section->elements;
    this->data.erase(dataStart, dataEnd);
    auto sectionIter = this->sections.begin() + section->index;
    for (auto nextIter = sectionIter++; nextIter != this->sections.end(); ++nextIter) {
        (*nextIter)->index--;
        (*nextIter)->elementIndex -= section->elements;
    }
    this->sections.erase(sectionIter);
}


#endif //BUILDIT_VERTEXBUFFER_H
