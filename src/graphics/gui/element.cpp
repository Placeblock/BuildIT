//
// Created by felix on 8/14/24.
//

#include <GL/glew.h>
#include <algorithm>
#include "element.h"
#include <string>
#include <utility>
#include "graphics/data/program.h"
#include "graphics/font/fontDataLoader.h"

using namespace GUI;

void Element::setBufferSize(int delta) {
    this->parent->updateBufferSizeRecursive(this, delta);
}

void Element::updateBufferSizeRecursive(Element* child, int delta) {
    this->childrenBufferSize += delta;

    auto iter = std::find_if(this->children.begin(), this->children.end(), [&](const std::unique_ptr<Element>& w) {
        return child == w.get();
    });
    while (++iter != this->children.end()) {
        (*iter)->moveBufferIndexRecursive(delta);
    }

    if (this->parent != nullptr) {
        this->parent->updateBufferSizeRecursive(this, delta);
    }
}

void Element::moveBufferIndexRecursive(int delta) {
    this->bufferIndex += delta;
    for (const auto &child: this->children) {
        child->moveBufferIndexRecursive(delta);
    }
}

void Element::addChild(std::unique_ptr<Element>& child) {
    child->parent = this;
    if (this->children.empty()) {
        child->setBufferIndex(this->bufferIndex + this->calcBufferSize());
    } else {
        child->setBufferIndex(this->children.back()->bufferIndex + this->children.back()->getRequiredBufferSpace());
    }
    this->childrenBufferSize += child->getRequiredBufferSpace();
    if (this->parent != nullptr) {
        this->parent->updateBufferSizeRecursive(this, int(child->getRequiredBufferSpace()));
    }
    this->children.push_back(std::move(child));
}

void Element::removeChild(Element *child) {
    auto iter = std::find_if(this->children.begin(), this->children.end(), [&child](const std::unique_ptr<Element>& c){
        return c.get() == child;
    });
    this->updateBufferSizeRecursive(iter->get(), -int(child->getRequiredBufferSpace()));
    this->children.erase(iter);
}

void Element::setBufferIndex(uint index) {
    this->bufferIndex = index;
    index += this->calcBufferSize();
    for (const auto &child: this->children) {
        child->setBufferIndex(index);
        index += child->getRequiredBufferSpace();
    }
}

void Element::updateSize(uintVec2 newSize) {
    if (this->size == newSize) return;
    this->size = newSize;
    if (this->parent != nullptr) {
        this->parent->onChildUpdateSize(this);
    }
    for (const auto &child: this->children) {
        child->onParentUpdateSize();
    }
}

void Element::prerender(Programs* programs) {
    this->rendered = true;
    for (const auto &child: this->children) {
        child->prerender(programs);
    }
}

void Element::postrender(Programs* programs) {
    for (const auto &child: this->children) {
        child->postrender(programs);
    }
}

void Element::updatePos(uintVec2 newRelPos) {
    this->relPos = newRelPos;
    if (this->parent != nullptr) {
        this->absPos = this->parent->absPos + newRelPos;
    } else {
        this->absPos = newRelPos;
    }
}

Font loadFont() {
    FontDataLoader fontDataLoader{"resources/font/data.fnt"};
    fontDataLoader.load();
    FontLoader fontLoader{fontDataLoader.fontData};
    fontLoader.load();
    return fontLoader.font;
}

View::View(Programs *programs) : programs(programs),
        font(loadFont()), fontMetrics({this->font.data}), fontRenderer(FontRenderer(this->font)),
                                 vertexBuffer({GL_ARRAY_BUFFER,
                                               BufferLayout{BufferLayoutElement{GL_FLOAT, 2, false}}}),
                                 texBuffer({GL_ARRAY_BUFFER,
                                            BufferLayout{BufferLayoutElement{GL_FLOAT, 2, false}}}),
                                 colorBuffer({GL_ARRAY_BUFFER,
                                              BufferLayout{BufferLayoutElement{GL_UNSIGNED_BYTE, 4, true}}}){
    this->vertexArray.addBuffer(&this->vertexBuffer);
    this->vertexArray.addBuffer(&this->texBuffer);
    this->vertexArray.addBuffer(&this->colorBuffer);
}

void View::regenerateBuffers() {
    const uint space = this->root->getRequiredBufferSpace();
    std::vector<glm::vec2> vertexData;
    vertexData.reserve(space);
    std::vector<glm::vec2> texData;
    texData.reserve(space);
    std::vector<Color> colorData;
    colorData.reserve(space);
    this->textures.clear();
    this->textures.reserve(space);
    this->root->generateBuffer(vertexData, texData, colorData, textures);
    this->vertexBuffer.bufferData(vertexData);
    this->texBuffer.bufferData(texData);
    this->colorBuffer.bufferData(colorData);
}

void View::render() {
    this->root->prerender(this->programs);
    programs->textureProgram->use();
    this->vertexArray.bind();
    const uintVec2 size = this->root->getSize();
    this->programs->updateProjectionUniforms(size, this->camera);
    this->programs->textureProgram->use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, int(size.x), int(size.y));

    auto startIter = this->textures.begin();
    auto endIter = this->textures.begin();
    int start = 0;
    int count = 0;
    while (endIter++ != this->textures.end()) {
        count++;

        if (endIter == this->textures.end() || *endIter != *startIter) {
            glBindTexture(GL_TEXTURE_2D, *startIter);
            glDrawArrays(GL_TRIANGLES, start, count);

            startIter = endIter;
            start += count;
            count = 0;
        }
    }
    this->root->postrender(programs);
    this->fontRenderer.render(programs->textureProgram);
}

void View::updateVertices(Element* element, const std::vector<glm::vec2> &vertices) {
    if (!element->rendered) return;
    uint vIndex = element->getBufferIndex();
    this->vertexBuffer.bind();
    this->vertexBuffer.bufferSubData(vIndex, vertices);
}

void View::updateColors(Element* element, const std::vector<Color> &colors) {
    uint cIndex = element->getBufferIndex();
    this->colorBuffer.bind();
    this->colorBuffer.bufferSubData(cIndex, colors);
}

void View::moveMouse(glm::vec2 newPos) {
    glm::vec2 delta = newPos - this->mousePos;
    this->root->onMouseMove(newPos, delta);
    this->mousePos = newPos;
}
