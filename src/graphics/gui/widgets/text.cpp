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

Text::Text(View *view, uintVec2 size, const std::string& text, Alignment alignment, Color color, uint fontSize)
    : Element(view, calcSize(size, text, &view->fontMetrics, fontSize)), text(text), alignment(alignment), color(color), fontSize(fontSize) {
    TextData data = view->fontMetrics.generateTextData(this->text, this->alignment, intVec2(), this->fontSize, this->color);
    this->vertexCount = data.vertices.size()/2;
}

void Text::generateBuffer(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                  std::vector<uint> &textures) {
    uintVec2 textPos = this->getRelPos();
    if (this->alignment == Alignment::CENTER) {
        textPos.x += this->getSize().x/2;
    } else if (this->alignment == Alignment::RIGHT) {
        textPos.x += this->getSize().x;
    }
    TextData data = this->view->fontMetrics.generateTextData(this->text, this->alignment, intVec2(this->getAbsPos()), this->fontSize, this->color);
    vertices.insert(vertices.end(), data.vertices.begin(), data.vertices.end());
    texCoords.insert(texCoords.end(), data.texCoords.begin(), data.texCoords.end());
    colors.insert(colors.end(), data.colors.begin(), data.colors.end());
    const std::vector<uint> textTextures(this->vertexCount, this->view->font.texture);
    textures.insert(textures.end(), textTextures.begin(), textTextures.end());
}

uint Text::calcBufferSize() const {
    return this->vertexCount;
}

void Text::updatePos(uintVec2 newPos) {
    Element::updatePos(newPos);
    if (this->rendered) {
        TextData data = this->view->fontMetrics.generateTextData(this->text, this->alignment, intVec2(this->getRelPos()), this->fontSize, this->color);
        this->view->updateVertices(this, data.vertices);
    }
}
