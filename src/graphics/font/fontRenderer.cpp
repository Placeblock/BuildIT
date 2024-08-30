//
// Created by felix on 8/11/24.
//

#include <algorithm>
#include <iostream>
#include <utility>
#include "fontRenderer.h"

void FontRenderer::render(Program *program) {
    program->use();
    this->vertexArray.bind();
    glBindTexture(GL_TEXTURE_2D, this->font.texture);
    glDrawArrays(GL_TRIANGLES, 0, this->vertexBuffer.getSize());
}

BufferLayout getCharLayout() {
    BufferLayout layout;
    layout.addElement({GL_FLOAT, 2, false});
    layout.addElement({GL_FLOAT, 2, false});
    layout.addElement({GL_UNSIGNED_BYTE, 3, true});
    return layout;
}

FontRenderer::FontRenderer(const Font& font)
        : font(font), metrics(FontMetrics{font.data}), vertexBuffer({GL_ARRAY_BUFFER, getCharLayout()}) {
    this->vertexArray.addBuffer(&this->vertexBuffer);
}

std::unique_ptr<RenderedText> FontRenderer::addText(const std::string& text, Alignment alignment, glm::vec2 pos, uint fontSize, Color color) {
    std::vector<CharVertex> textData = this->metrics.generateTextData(text, alignment, pos, fontSize, color);

    BufferSection *bufferSection = this->vertexBuffer.addElement(textData);
    this->updateBuffers();

    return std::make_unique<RenderedText>(text, alignment, fontSize, color, bufferSection);
}

void FontRenderer::removeText(RenderedText *data) {
    this->vertexBuffer.removeSection(data->bufferSection);
    this->updateBuffers();
}

void FontRenderer::updateBuffers() {
    this->vertexBuffer.bufferAll();
}

void FontRenderer::moveText(RenderedText *data, glm::vec2 newPos) {
    std::vector<CharVertex> textData = this->metrics.generateTextData(data->text, data->alignment, newPos, data->fontSize, data->color);
    this->vertexBuffer.bind();
    this->vertexBuffer.bufferSection(data->bufferSection, textData);
}
