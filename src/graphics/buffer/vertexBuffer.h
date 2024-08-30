//
// Created by felix on 8/30/24.
//

#ifndef BUILDIT_VERTEXBUFFER_H
#define BUILDIT_VERTEXBUFFER_H


#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>

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
public:
    unsigned int stride = 0;
    virtual std::vector<BufferLayoutElement> getElements() = 0;
    [[nodiscard]] virtual unsigned int getSize() const = 0;
};

class MultiElementBufferLayout : public BufferLayout {
private:
    std::vector<BufferLayoutElement> elements;
public:
    void addElement(BufferLayoutElement element);
    std::vector<BufferLayoutElement> getElements() override;
    [[nodiscard]] unsigned int getSize() const override;
};

class SingleElementBufferLayout : public BufferLayout {
private:
    BufferLayoutElement element;
public:
    explicit SingleElementBufferLayout(BufferLayoutElement& element) : element(element) {};
    std::vector<BufferLayoutElement> getElements() override;
};

struct RenderedElement {
    size_t index;
};

template<typename T>
class VertexBuffer {
public:
    VertexBuffer(unsigned int type, BufferLayout *layout);
    VertexBuffer(unsigned int type, BufferLayout *layout, const std::vector<T> &data);
    unsigned int type;
    unsigned int id = 0;
    BufferLayout *layout;
    void bind();
    void bufferData(const std::vector<T>& data);
    void bufferSubData(unsigned int offset, const T& data);
    void bufferSubData(unsigned int offset, const std::vector<T>& data);

    virtual ~VertexBuffer();
};
template<typename T>
VertexBuffer<T>::VertexBuffer(unsigned int type, BufferLayout *layout, const std::vector<T> &data)
    : layout(layout), type(type) {
    glGenBuffers(1, &this->id);
    glBindBuffer(this->type, this->id);
    glBufferData(this->type, this->layout->getSize() * data.size(), data.data());
}

template<typename T>
VertexBuffer<T>::VertexBuffer(unsigned int type, BufferLayout *layout) : layout(layout), type(type) {
    glGenBuffers(1, &this->id);
    glBindBuffer(this->type, this->id);
}

template<typename T>
VertexBuffer<T>::~VertexBuffer() {
    glDeleteBuffers(1, &this->id);
}

template<typename T>
void VertexBuffer<T>::bind() {
    glBindBuffer(this->type, this->ID);
}

template<typename T>
void VertexBuffer<T>::bufferData(const std::vector<T> &data) {
    this->bind();
    glBufferData(this->type, this->layout->getSize() * data.size(), data.data());
}

template<typename T>
void VertexBuffer<T>::bufferSubData(unsigned int offset, const T &data) {
    glBufferSubData(this->type, this->layout->getSize()*offset, this->layout->getSize(), data);
}

template<typename T>
void VertexBuffer<T>::bufferSubData(unsigned int offset, const std::vector<T> &data) {
    glBufferSubData(this->type, this->layout->getSize()*offset, this->layout->getSize() * data.size(), data.data());
}

template<typename T>
class IndexedBuffer : public VertexBuffer<T> {
protected:
    std::vector<T> data;
    std::vector<std::unique_ptr<RenderedElement>> elements;
public:
    virtual RenderedElement* addElement(T data);
    virtual void removeElement(RenderedElement *element);
};

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
class SectionedBuffer : public IndexedBuffer<T> {
    std::vector<std::unique_ptr<BufferSection>> sections;
    RenderedElement* addElement(T data) override;
    RenderedElement* addElement(T data, BufferSection *section);
    void removeElement(RenderedElement *element) override;
    void bufferSection(BufferSection *section, const std::vector<T> &data);
};

template<typename T>
void SectionedBuffer<T>::bufferSection(BufferSection *section, const std::vector<T> &data) {
    assert(data.size() != section->elements && "Tried to write " + std::to_string(data.size()) +
        " elements to section which contains " + std::to_string(section->elements) + " elements");
    glBufferSubData(this->type, this->layout->getSize()*section->elementIndex, this->layout->getSize()*data.size(), data.data());
}

template<typename T>
inline RenderedElement *SectionedBuffer<T>::addElement(T data, BufferSection *section) {
    section->elements++;
    size_t index = section->index + section->elements;
    auto iter = this->elements.begin() + index;
    this->data.insert(this->data.begin() + index, data);
    auto renderedElement = new RenderedElement(index);
    this->elements.insert(iter, std::unique_ptr<RenderedElement>(renderedElement));
    while(++iter != this->elements.end()) {
        (*iter)->index++;
    }
    auto sIter = this->sections.begin() + section->index;
    while (++sIter != this->sections.end()) {
        (*sIter)->elementIndex++;
    }
    return renderedElement;
}

template<typename T>
inline void SectionedBuffer<T>::removeElement(RenderedElement *element) {
    for (const auto eIter = this->elements.begin() + element->index + 1; eIter != this->elements.end(); ++eIter) {
        (*eIter)->index--;
    }
    const auto sIter = std::find_if(this->sections.begin(), this->sections.end(), [&element](const BufferSection& section) {
        return section.elementIndex <= element->index && section.elementIndex + section.elements > element->index;
    });
    bool eraseSection = --*sIter->elements == 0;
    if (eraseSection) {
        sIter = this->sections.erase(sIter);
    } else {
        sIter++;
    }
    while (sIter != this->sections.end()) {
        *sIter->elementIndex--;
        if (eraseSection) *sIter->index--;
        sIter++;
    }
    IndexedBuffer<T>::removeElement(element);
}

template<typename T>
inline RenderedElement *SectionedBuffer<T>::addElement(T data) {
    this->sections.push_back(std::make_unique<BufferSection>(this->sections.size(), 1, this->elements.size()));
    return this->addElement(data, this->sections.back().get());
}


#endif //BUILDIT_VERTEXBUFFER_H
