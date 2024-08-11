//
// Created by felix on 8/11/24.
//

#include <algorithm>
#include <iostream>
#include "fontRenderer.h"

void FontRenderer::render(Program *program) {
    program->use();
    glBindVertexArray(this->vAO);
    glBindTexture(GL_TEXTURE_2D, this->loader.textures[0]);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}

FontRenderer::FontRenderer(FontMetrics metrics, FontLoader loader) : metrics(std::move(metrics)), loader(std::move(loader)) {
    glGenVertexArrays(1, &this->vAO);
    glBindVertexArray(this->vAO);

    glGenBuffers(2, this->vBOs);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->texCoords.size()*sizeof(float), this->texCoords.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(1);
}

RenderedText FontRenderer::addText(const std::string& text, Alignment alignment, glm::vec2 pos) {
    TextData textData = this->metrics.generateTextData(text, alignment, pos);
    uint offset = this->vertices.size()/12;
    uint size = text.size();

    this->vertices.insert(this->vertices.end(), textData.vertices.begin(), textData.vertices.end());
    this->texCoords.insert(this->texCoords.end(), textData.texCoords.begin(), textData.texCoords.end());
    this->updateBuffers();

    RenderedText renderedText{offset, size};
    this->renderedTexts.insert(renderedText);
    return renderedText;
}

void FontRenderer::removeText(RenderedText data) {
    this->vertices.erase(this->vertices.begin() + data.offset*12, this->vertices.begin() + data.offset*12 + data.size*12);
    this->texCoords.erase(this->texCoords.begin() + data.offset*12, this->texCoords.begin() + data.offset*12 + data.size*12);
    this->updateBuffers();
    auto iter = this->renderedTexts.find(data);
    iter = this->renderedTexts.erase(iter);
    for (; iter != this->renderedTexts.end(); ++iter) {
        RenderedText text = *iter;
        iter = this->renderedTexts.erase(iter);
        text.offset -= data.size;
        this->renderedTexts.insert(text);
    }
}

void FontRenderer::updateBuffers() {
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(float), this->vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->vBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, this->texCoords.size()*sizeof(float), this->texCoords.data(), GL_DYNAMIC_DRAW);
}
