//
// Created by felix on 8/15/24.
//

#include "text.h"
#include <sys/types.h>
#include <iostream>
#include "graphics/font/fontDataLoader.h"

using namespace GUI;

uintVec2 calcSize(uintVec2 size, const std::string& text, FontMetrics* metrics, uint fontSize) {
    const unsigned int lines = uint(FontMetrics::splitLines(text).size());
    const float scaleFactor = metrics->getScaleFactor(fontSize);
    if (size.x == 0) {
        float width = metrics->calculateMaxTextWidth(text, scaleFactor);
        return {uint(width), lines*metrics->data.lineHeight};
    }
    size.y = uint(float(lines)*float(metrics->data.lineHeight)*scaleFactor);
    return size;
}

Text::Text(View *view, uintVec2 size, const std::string& text, Alignment alignment, Color color, unsigned int fontSize)
    : Element(view, calcSize(size, text, &view->fontMetrics, fontSize)), text(text), alignment(alignment), color(color), fontSize(fontSize) {
    std::vector<CharVertex> data = view->fontMetrics.generateTextData(this->text, this->alignment, intVec2(), this->fontSize, this->color);
    this->vertexCount = data.size();
}

void Text::generateBuffer(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                  std::vector<unsigned int> &textures) {
    uintVec2 textPos = this->getAbsPos();
    if (this->alignment == Alignment::CENTER) {
        textPos.x += this->getSize().x/2;
    } else if (this->alignment == Alignment::RIGHT) {
        textPos.x += this->getSize().x;
    }
    std::vector<CharVertex> data = this->view->fontMetrics.generateTextData(this->text, this->alignment, intVec2(textPos), this->fontSize, this->color);
    for (const auto &charVertex: data) {
        vertices.push_back(charVertex.pos.x);
        vertices.push_back(charVertex.pos.y);
        texCoords.push_back(charVertex.texCoord.x);
        texCoords.push_back(charVertex.texCoord.y);
        colors.push_back(charVertex.color.x);
        colors.push_back(charVertex.color.y);
        colors.push_back(charVertex.color.z);
    }
    const std::vector<unsigned int> textTextures(this->vertexCount, this->view->font.texture);
    textures.insert(textures.end(), textTextures.begin(), textTextures.end());
}

unsigned int Text::calcBufferSize() const {
    return this->vertexCount;
}

void Text::updatePos(uintVec2 newPos) {
    Element::updatePos(newPos);
    std::vector<CharVertex> data = this->view->fontMetrics.generateTextData(this->text, this->alignment, intVec2(this->getRelPos()), this->fontSize, this->color);
    std::vector<float> vertices;
    for (const auto &charVertex: data) {
        vertices.push_back(charVertex.pos.x);
        vertices.push_back(charVertex.pos.y);
    }
    this->view->updateVertices(this, vertices);
}
