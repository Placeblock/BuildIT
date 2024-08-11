//
// Created by felix on 8/11/24.
//

#include "fontMetrics.h"
#include "graphics/types.h"
#include "glm/detail/type_vec3.hpp"

glm::vec2 FontMetrics::getGlyphPos(intVec2 origin, Char cChar, float scaleFactor) {
    return glm::vec2(origin) + glm::vec2(cChar.offset) * scaleFactor;
}

TextData FontMetrics::generateTextData(const std::string& text, Alignment alignment, intVec2 pos, uint fontSize, Color color) {
    float scaleFactor = float(fontSize)/float(this->data.size);
    const uint width = this->calculateTextWidth(text) * scaleFactor;
    const int alignmentDelta = FontMetrics::calculateAlignmentDelta(width, alignment);
    pos.x += alignmentDelta;
    TextData textData;
    textData.vertices.reserve(text.size()*12);
    textData.texCoords.reserve(text.size()*12);
    textData.colors.reserve(text.size()*18);
    for (int i = 0; i < text.size()*6; ++i) {
        textData.colors.push_back(color.x);
        textData.colors.push_back(color.y);
        textData.colors.push_back(color.z);
    }

    for (const auto &textChar: text) {
        int unicode = (unsigned char) textChar;
        if (!this->data.chars.contains(unicode)) continue;
        Char fontChar = this->data.chars[unicode];
        const intVec2 glyphPos = this->getGlyphPos(pos, fontChar, scaleFactor);

        textData.vertices.push_back(float(glyphPos.x));
        textData.vertices.push_back(float(glyphPos.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.size.x * scaleFactor));
        textData.vertices.push_back(float(glyphPos.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.size.x * scaleFactor));
        textData.vertices.push_back(float(glyphPos.y + fontChar.size.y * scaleFactor));
        textData.vertices.push_back(float(glyphPos.x + fontChar.size.x * scaleFactor));
        textData.vertices.push_back(float(glyphPos.y + fontChar.size.y * scaleFactor));
        textData.vertices.push_back(float(glyphPos.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.size.y * scaleFactor));
        textData.vertices.push_back(float(glyphPos.x));
        textData.vertices.push_back(float(glyphPos.y));

        textData.texCoords.push_back(float(fontChar.pos.x)/float(this->data.bitmapSize.x));
        textData.texCoords.push_back(float(fontChar.pos.y)/float(this->data.bitmapSize.y));
        textData.texCoords.push_back(float(fontChar.pos.x + fontChar.size.x)/float(this->data.bitmapSize.x));
        textData.texCoords.push_back(float(fontChar.pos.y)/float(this->data.bitmapSize.y));
        textData.texCoords.push_back(float(fontChar.pos.x + fontChar.size.x)/float(this->data.bitmapSize.x));
        textData.texCoords.push_back(float(fontChar.pos.y + fontChar.size.y)/float(this->data.bitmapSize.y));
        textData.texCoords.push_back(float(fontChar.pos.x + fontChar.size.x)/float(this->data.bitmapSize.x));
        textData.texCoords.push_back(float(fontChar.pos.y + fontChar.size.y)/float(this->data.bitmapSize.y));
        textData.texCoords.push_back(float(fontChar.pos.x)/float(this->data.bitmapSize.x));
        textData.texCoords.push_back(float(fontChar.pos.y + fontChar.size.y)/float(this->data.bitmapSize.y));
        textData.texCoords.push_back(float(fontChar.pos.x)/float(this->data.bitmapSize.x));
        textData.texCoords.push_back(float(fontChar.pos.y)/float(this->data.bitmapSize.y));

        pos.x += fontChar.advance * scaleFactor;
    }

    return textData;
}

uint FontMetrics::calculateTextWidth(const std::string& text) {
    int width = 0;
    for (const auto &textChar: text) {
        int unicode = (unsigned char) textChar;
        if (!this->data.chars.contains(unicode)) continue;
        Char fontChar = this->data.chars[unicode];
        width += fontChar.advance;
    }
    return width;
}

int FontMetrics::calculateAlignmentDelta(uint textWidth, Alignment alignment) {
    switch (alignment) {
        case Alignment::LEFT:
            return 0;
        case Alignment::CENTER:
            return -int(textWidth)/2;
        case Alignment::RIGHT:
            return -int(textWidth);
    }
}
