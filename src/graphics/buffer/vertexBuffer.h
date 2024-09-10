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
#include <list>
#include "glm/vec2.hpp"

struct VertexData {
    glm::vec2 pos;
    Color color;
} __attribute__ ((packed));

struct BufferSection {
    size_t index;
    size_t elementIndex;
    unsigned int elements;
};

struct BufferLayoutElement {
    unsigned int type;
    unsigned int count;
    bool normalized;
    unsigned int divisor = 0;
public:
    [[nodiscard]] unsigned int getSize() const;
    static unsigned int getTypeSize(unsigned int type);
};

struct BufferLayout {
private:
    std::vector<BufferLayoutElement> elements;
public:
    BufferLayout();
    explicit BufferLayout(BufferLayoutElement element);
    void addElement(BufferLayoutElement element);
    std::vector<BufferLayoutElement> getElements();
};

struct Index {
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

/*
 * CACHED
 */

template<typename T>
class CachedVertexBuffer : public VertexBuffer<T> {
protected:
    std::list<T> data;
public:
    CachedVertexBuffer(unsigned int type, BufferLayout layout) : VertexBuffer<T>(type, layout) {};
    virtual void addData(T newData);
    virtual void addData(T newData, size_t index);
    virtual void removeData(size_t index);
    virtual void updateData(T newData, size_t index);
    void bufferAll();
    virtual void clear();
    size_t size();
};

template<typename T>
void CachedVertexBuffer<T>::updateData(T newData, size_t index) {
    glBufferSubData(this->type, sizeof(T)*index, sizeof(T), &newData);
}

template<typename T>
void CachedVertexBuffer<T>::bufferAll() {
    std::vector<T> rawData{this->data.begin(), this->data.end()};
    this->bufferData(rawData);
}

template<typename T>
void CachedVertexBuffer<T>::addData(T newData) {
    this->data.push_back(newData);
}

template<typename T>
void CachedVertexBuffer<T>::addData(T newData, size_t index) {
    this->data.insert(std::next(this->data.begin(), index), newData);
}

template<typename T>
void CachedVertexBuffer<T>::removeData(size_t index) {
    this->data.erase(std::next(this->data.begin(), index));
}

template<typename T>
void CachedVertexBuffer<T>::clear() {
    this->data.clear();
}

template<typename T>
size_t CachedVertexBuffer<T>::size() {
    return this->data.size();
}

/*
 * INDEXED
 */

class Indexed {
protected:
    std::vector<std::unique_ptr<Index>> elements;
public:
    Index* addElement(size_t index);
    void removeElement(Index *element);
    void clear();
};

void Indexed::clear() {
    this->elements.clear();
}

inline void Indexed::removeElement(Index *element) {
    this->elements.erase(this->elements.begin() + element->index);
}

Index *Indexed::addElement(size_t index) {
    this->elements.insert(this->elements.begin() + index, std::make_unique<Index>(index));
    return this->elements[index].get();
}

template<typename T>
class IndexedBuffer : public CachedVertexBuffer<T> {
protected:
    Indexed indexed;
public:
    IndexedBuffer(unsigned int type, BufferLayout layout) : VertexBuffer<T>(type, layout) {};
    Index* addElement(T newData);
    Index* addElement(T newData, size_t index);
    void removeElement(Index *element);
    void clear() override;
};

template<typename T>
void IndexedBuffer<T>::clear() {
    CachedVertexBuffer<T>::clear();
    this->indexed.clear();
}

template<typename T>
inline void IndexedBuffer<T>::removeElement(Index *element) {
    this->removeData(element->index);
    this->indexed.removeElement(element);
}

template<typename T>
inline Index *IndexedBuffer<T>::addElement(T newData) {
    Index *index = this->indexed.addElement(this->data.size());
    this->addData(newData);
    return index;
}

template<typename T>
Index *IndexedBuffer<T>::addElement(T newData, size_t i) {
    Index *index = this->indexed.addElement(index);
    this->addData(newData, i);
    return index;
}


/*
 * SECTIONED
 */

class Sectioned {
protected:
    std::list<std::unique_ptr<BufferSection>> sections;
public:
    BufferSection* createSection(unsigned int elementIndex, unsigned int size = 1);
    void addElement(BufferSection *section);
    void removeElement(BufferSection *section);
    void removeSection(BufferSection *section);
    void clear();
};

BufferSection *Sectioned::createSection(unsigned int elementIndex, unsigned int size) {
    this->sections.push_back(std::make_unique<BufferSection>(this->sections.size(), elementIndex, size));
    return this->sections.back().get();
}

void Sectioned::addElement(BufferSection *section) {
    section->elements++;
    auto sIter = std::next(this->sections.begin(), section->index);
    while (++sIter != this->sections.end()) {
        (*sIter)->elementIndex++;
    }
}

void Sectioned::removeSection(BufferSection *section) {
    auto sectionIter = std::next(this->sections.begin(), section->index);
    for (auto nextIter = sectionIter++; nextIter != this->sections.end(); ++nextIter) {
        (*nextIter)->index--;
        (*nextIter)->elementIndex -= section->elements;
    }
    this->sections.erase(sectionIter);
}

void Sectioned::clear() {
    this->sections.clear();
}

void Sectioned::removeElement(BufferSection *section) {
    auto sectionIter = std::next(this->sections.begin(), section->index);
    section->elements--;
    if (section->elements == 0) {
        sectionIter = this->sections.erase(sectionIter);
    } else {
        sectionIter++;
    }
    while (sectionIter != this->sections.end()) {
        (*sectionIter)->elementIndex--;
        sectionIter++;
    }
}


template<typename T>
class SectionedBuffer : public CachedVertexBuffer<T> {
protected:
    Sectioned sectioned;
public:
    SectionedBuffer(unsigned int type, BufferLayout layout) : CachedVertexBuffer<T>(type, layout) {};
    BufferSection* addElement(T newData);
    BufferSection* addElements(const std::vector<T> & newData);
    BufferSection* removeElement(BufferSection *section, unsigned int sectionIndex);
    BufferSection* createSection();
    void addElement(T newData, BufferSection *section);
    void removeSection(BufferSection *section);
    void updateSection(BufferSection *section, const std::vector<T> &newData);
    void updateElement(T newData, BufferSection *section, unsigned int sectionIndex);
    void clear() override;
};

template<typename T>
BufferSection *SectionedBuffer<T>::createSection() {
    return this->sectioned.createSection(this->data.size(), 0);
}

template<typename T>
void SectionedBuffer<T>::updateElement(T newData, BufferSection *section, unsigned int sectionIndex) {
    unsigned int index = section->elementIndex + sectionIndex;
    this->updateData(newData, index);
}

template<typename T>
BufferSection *SectionedBuffer<T>::removeElement(BufferSection *section, unsigned int sectionIndex) {
    unsigned int index = section->elementIndex + sectionIndex;
    this->removeData(index);
    this->sectioned.removeElement(section);
}

template<typename T>
void SectionedBuffer<T>::clear() {
    CachedVertexBuffer<T>::clear();
    this->sectioned.clear();
}

template<typename T>
void SectionedBuffer<T>::updateSection(BufferSection *section, const std::vector<T> &newData) {
    assert((("Tried to write " + std::to_string(newData.size()) +
            " elements to section which contains " + std::to_string(section->elements) + " elements").c_str(), newData.size() == section->elements));
    glBufferSubData(this->type, sizeof(T)*section->elementIndex, sizeof(T) * newData.size(), newData.data());
}

template<typename T>
inline void SectionedBuffer<T>::addElement(T newData, BufferSection *section) {
    size_t index = section->index + section->elements;
    this->sectioned.addElement(section);
    this->addData(newData, index);
}

template<typename T>
inline BufferSection* SectionedBuffer<T>::addElement(T newData) {
    BufferSection *section = this->sectioned.createSection(this->data.size());
    this->addData(newData);
    return section;
}

template<typename T>
BufferSection *SectionedBuffer<T>::addElements(const std::vector<T> &newData) {
    BufferSection *section = this->sectioned.createSection(this->data.size(), newData.size());
    this->data.insert(this->data.end(), newData.begin(), newData.end());
    return section;
}

template<typename T>
void SectionedBuffer<T>::removeSection(BufferSection *section) {
    const auto dataStart = std::next(this->data.begin(), section->elementIndex);
    const auto dataEnd = std::next(this->data.begin(), section->elementIndex + section->elements);
    this->data.erase(dataStart, dataEnd);
    this->sectioned.removeSection(section);
}

#endif //BUILDIT_VERTEXBUFFER_H
