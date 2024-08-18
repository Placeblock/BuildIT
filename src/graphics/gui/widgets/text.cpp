//
// Created by felix on 8/15/24.
//

#include "text.h"

#include <utility>

Text::Text(GUI *gui, uintVec2 size, FontMetrics* metrics, Font* font, std::string text, Alignment alignment, Color color, uint fontSize)
    : Element(gui, size), text(std::move(text)), metrics(metrics), font(font), alignment(alignment), color(color), fontSize(fontSize) {
    if (size.x == 0) {
        float width = this->metrics->calculateMaxTextWidth(text, this->metrics->getScaleFactor(fontSize));
        this->size.x = uint(width);
        this->size.y = uint(FontMetrics::splitLines(text).size())*this->metrics->data.lineHeight;
    }
}

void Text::render(uintVec2 pos, std::list<float> &vertices, std::list<float> &texCoords, std::list<Color> &colors,
                  std::list<uint> &texture) {
    uintVec2 textPos = pos;
    if (this->alignment == Alignment::CENTER) {
        textPos.x += this->size.x/2;
    } else if (this->alignment == Alignment::RIGHT) {
        textPos.x += this->size.x;
    }
    TextData data = this->metrics->generateTextData(this->text, this->alignment, intVec2(pos), this->fontSize, this->color);
    vertices.insert(vertices.end(), data.vertices.begin(), data.vertices.end());
    texCoords.insert(texCoords.end(), data.texCoords.begin(), data.texCoords.end());
    colors.insert(colors.end(), data.colors.begin(), data.colors.end());
    const std::vector<uint> textures(data.vertices.size(), this->font->texture);
    texture.insert(texture.end(), textures.begin(), textures.end());
}
