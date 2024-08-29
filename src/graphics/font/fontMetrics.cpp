//
// Created by felix on 8/11/24.
//

#include <sstream>
#include "fontMetrics.h"
#include "graphics/types.h"

glm::vec2 FontMetrics::getGlyphPos(intVec2 origin, Char cChar, float scaleFactor) {
    return glm::vec2(origin) + glm::vec2(cChar.offset) * scaleFactor;
}

TextData FontMetrics::generateTextData(const std::string& text, Alignment alignment, intVec2 initPos, uint fontSize, Color color) {
    float scaleFactor = this->getScaleFactor(fontSize);
    const std::vector<float> lineWidths = this->calculateTextWidth(text, scaleFactor);
    const std::vector<std::string> lines = FontMetrics::splitLines(text);

    TextData textData;
    textData.vertices.reserve(text.size()*12);
    textData.texCoords.reserve(text.size()*12);
    textData.colors.reserve(text.size()*18);
    for (int i = 0; i < text.size()*6; ++i) {
        textData.colors.push_back(color.x);
        textData.colors.push_back(color.y);
        textData.colors.push_back(color.z);
    }

    intVec2 pos = initPos;
    for (int i = 0; i < lines.size(); ++i) {
        const float alignmentDelta = FontMetrics::calculateAlignmentDelta(lineWidths[i], alignment);
        pos.x = int(float(initPos.x) + alignmentDelta);

        for (const auto &textChar: lines[i]) {
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

        pos.y += int(float(this->data.lineHeight) * scaleFactor);
    }

    return textData;
}

std::vector<float> FontMetrics::calculateTextWidth(const std::string& text, float scaleFactor) {
    std::vector<float> lines = {0};
    for (const auto &textChar: text) {
        if (textChar == '\n') {
            lines.push_back(0);
        }
        int unicode = (unsigned char) textChar;
        if (!this->data.chars.contains(unicode)) continue;
        Char fontChar = this->data.chars[unicode];
        lines.back() += float( fontChar.advance) * scaleFactor;
    }
    return lines;
}

float FontMetrics::calculateMaxTextWidth(const std::string &text, float scaleFactor) {
    float width = 0;
    float lineWidth = 0;
    for (const auto &textChar: text) {
        if (textChar == '\n') {
            lineWidth = 0;
        }
        int unicode = (unsigned char) textChar;
        if (!this->data.chars.contains(unicode)) continue;
        Char fontChar = this->data.chars[unicode];
        lineWidth += float( fontChar.advance) * scaleFactor;
        if (lineWidth > width) width = lineWidth;
    }
    return width;
}


float FontMetrics::calculateAlignmentDelta(float textWidth, Alignment alignment) {
    switch (alignment) {
        case Alignment::LEFT:
            return 0;
        case Alignment::CENTER:
            return -textWidth/2;
        case Alignment::RIGHT:
            return -textWidth;
    }
}

std::vector<std::string> FontMetrics::splitLines(const std::string &text) {
    std::vector<std::string> result;
    std::stringstream ss (text);
    std::string item;
    while (std::getline(ss, item, '\n')) {
        result.push_back(item);
    }
    return result;
}

float FontMetrics::getScaleFactor(uint fontSize) const {
    return float(fontSize)/float(this->data.size);;
}