//
// Created by felix on 8/15/24.
//

#include "text.h"

using namespace GUI;

uintVec2 calcSize(uintVec2 size, const std::string& text, FontMetrics* metrics, uint fontSize) {
    const uint lines = uint(FontMetrics::splitLines(text).size());
    if (size.x == 0) {
        float width = metrics->calculateMaxTextWidth(text, metrics->getScaleFactor(fontSize));
        return {uint(width), lines*metrics->data.lineHeight};
    }
    size.y = lines*metrics->data.lineHeight;
    return size;
}

Text::Text(View *view, uintVec2 size, FontMetrics* metrics, Font* font, const std::string& text, Alignment alignment, Color color, uint fontSize, Element* parent)
    : Element(view, calcSize(size, text, metrics, fontSize), parent), text(text), metrics(metrics), font(font), alignment(alignment), color(color), fontSize(fontSize) {
    TextData data = this->metrics->generateTextData(this->text, this->alignment, intVec2(), this->fontSize, this->color);
    this->vertexCount = data.vertices.size();
}

void Text::render(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                  std::vector<uint> &textures) {
    uintVec2 textPos = this->getPos();
    if (this->alignment == Alignment::CENTER) {
        textPos.x += this->getSize().x/2;
    } else if (this->alignment == Alignment::RIGHT) {
        textPos.x += this->getSize().x;
    }
    TextData data = this->metrics->generateTextData(this->text, this->alignment, intVec2(this->getPos()), this->fontSize, this->color);
    vertices.insert(vertices.end(), data.vertices.begin(), data.vertices.end());
    texCoords.insert(texCoords.end(), data.texCoords.begin(), data.texCoords.end());
    colors.insert(colors.end(), data.colors.begin(), data.colors.end());
    const std::vector<uint> textTextures(data.vertices.size(), this->font->texture);
    textures.insert(textures.end(), textTextures.begin(), textTextures.end());
}

uint Text::calcBufferSize() const {
    return this->vertexCount;
}
