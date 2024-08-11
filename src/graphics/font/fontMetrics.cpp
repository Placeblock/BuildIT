//
// Created by felix on 8/11/24.
//

#include "fontMetrics.h"
#include "graphics/types.h"

glm::vec2 FontMetrics::getGlyphPos(intVec2 origin, Char cChar) {
    return origin + cChar.offset;
}

TextData FontMetrics::generateTextData(const std::string& text, Alignment alignment, intVec2 pos) {
    const uint width = this->calculateTextWidth(text);
    const int alignmentDelta = FontMetrics::calculateAlignmentDelta(width, alignment);
    pos.x += alignmentDelta;
    TextData textData;
    textData.vertices.reserve(text.size()*12);
    textData.texCoords.reserve(text.size()*12);

    for (const auto &textChar: text) {
        int unicode = (unsigned char) textChar;
        if (!this->data.chars.contains(unicode)) continue;
        Char fontChar = this->data.chars[unicode];
        const intVec2 glyphPos = this->getGlyphPos(pos, fontChar);

        textData.vertices.push_back(float(glyphPos.x + fontChar.offset.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.offset.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.size.x + fontChar.offset.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.offset.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.size.x + fontChar.offset.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.size.y + fontChar.offset.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.size.x + fontChar.offset.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.size.y + fontChar.offset.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.offset.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.size.y + fontChar.offset.y));
        textData.vertices.push_back(float(glyphPos.x + fontChar.offset.x));
        textData.vertices.push_back(float(glyphPos.y + fontChar.offset.y));

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

        pos.x += fontChar.advance;
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
