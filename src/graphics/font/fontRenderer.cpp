//
// Created by felix on 8/11/24.
//

#include <algorithm>
#include <iostream>
#include <utility>
#include "fontRenderer.h"

void FontRenderer::render(Program *program) {
    program->use();
    glBindVertexArray(this->vAO);
    glBindTexture(GL_TEXTURE_2D, this->font.texture);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}

FontRenderer::FontRenderer(const Font& font) : font(font), metrics(FontMetrics{font.data}) {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(3, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->texCoords.size()*sizeof(float), this->texCoords.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size(), this->colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)nullptr);
    glEnableVertexAttribArray(2);
}

std::shared_ptr<RenderedText> FontRenderer::addText(const std::string& text, Alignment alignment, glm::vec2 pos, uint fontSize, Color color) {
    TextData textData = this->metrics.generateTextData(text, alignment, pos, fontSize, color);
    uint offset = this->vertices.size()/12;
    uint textLength = text.length();

    this->vertices.insert(this->vertices.end(), textData.vertices.begin(), textData.vertices.end());
    this->texCoords.insert(this->texCoords.end(), textData.texCoords.begin(), textData.texCoords.end());
    this->colors.insert(this->colors.end(), textData.colors.begin(), textData.colors.end());
    this->updateBuffers();

    std::shared_ptr<RenderedText> renderedText = std::make_shared<RenderedText>(text, alignment, offset, textLength, fontSize);
    this->renderedTexts.push_back(renderedText);
    return renderedText;
}

void FontRenderer::removeText(const std::shared_ptr<RenderedText>& data) {
    this->vertices.erase(this->vertices.begin() + data->offset*12, this->vertices.begin() + data->offset*12 + data->dataSize * 12);
    this->texCoords.erase(this->texCoords.begin() + data->offset*12, this->texCoords.begin() + data->offset*12 + data->dataSize * 12);
    this->colors.erase(this->colors.begin() + data->offset*18, this->colors.begin() + data->offset*18 + data->dataSize * 18);
    this->updateBuffers();
    auto iter = std::find(this->renderedTexts.begin(), this->renderedTexts.end(), data);
    iter = this->renderedTexts.erase(iter);
    for (; iter != this->renderedTexts.end(); ++iter) {
        (*iter)->offset -= data->dataSize;
    }
}

void FontRenderer::updateBuffers() {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->texCoords.size()*sizeof(float), this->texCoords.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size(), this->colors.data(), GL_DYNAMIC_DRAW);
}

void FontRenderer::moveText(const std::shared_ptr<RenderedText> &data, glm::vec2 newPos) {
    TextData textData = this->metrics.generateTextData(data->text, data->alignment, newPos, data->fontSize, data->color);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, data->offset*12, textData.vertices.size()*sizeof(float), textData.vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, data->offset*12, textData.texCoords.size()*sizeof(float), textData.texCoords.data());
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[2]);
    glBufferSubData(GL_ARRAY_BUFFER, data->offset*18, textData.colors.size(), textData.colors.data());
}
