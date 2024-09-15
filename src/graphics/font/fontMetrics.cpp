//
// Created by felix on 8/11/24.
//

#include <sstream>
#include "fontMetrics.h"
#include "graphics/types.h"

glm::vec2 FontMetrics::getGlyphPos(intVec2 origin, Char cChar, float scaleFactor) {
    return glm::vec2(origin) + glm::vec2(cChar.offset) * scaleFactor;
}

std::vector<CharVertex> FontMetrics::generateTextData(const std::string& text, Alignment alignment, glm::vec2 initPos, uint fontSize, Color color) {
    float scaleFactor = this->getScaleFactor(fontSize);
    const std::vector<float> lineWidths = this->calculateTextWidth(text, scaleFactor);
    const std::vector<std::string> lines = FontMetrics::splitLines(text);

    std::vector<CharVertex> textData;
    textData.reserve(text.size()*6);

    glm::vec2 pos = initPos;
    for (int i = 0; i < lines.size(); ++i) {
        const float alignmentDelta = FontMetrics::calculateAlignmentDelta(lineWidths[i], alignment);
        pos.x = float(initPos.x) + alignmentDelta;

        for (const auto &textChar: lines[i]) {
            int unicode = (unsigned char) textChar;
            if (!this->data.chars.contains(unicode)) continue;
            Char fontChar = this->data.chars[unicode];
            const glm::vec2 glyphPos = FontMetrics::getGlyphPos(pos, fontChar, scaleFactor);

            textData.emplace_back(glyphPos, glm::vec2(fontChar.pos)/glm::vec2(this->data.bitmapSize), color);
            textData.emplace_back(glyphPos + glm::vec2(float(fontChar.size.x) * scaleFactor, 0),
                                  glm::vec2(fontChar.pos.x + fontChar.size.x, fontChar.pos.y)/glm::vec2(this->data.bitmapSize), color);
            textData.emplace_back(glyphPos + glm::vec2(float(fontChar.size.x) * scaleFactor, float(fontChar.size.y) * scaleFactor),
                                  glm::vec2(fontChar.pos + fontChar.size)/glm::vec2(this->data.bitmapSize), color);
            textData.emplace_back(glyphPos + glm::vec2(float(fontChar.size.x) * scaleFactor, float(fontChar.size.y) * scaleFactor),
                                  glm::vec2(fontChar.pos + fontChar.size)/glm::vec2(this->data.bitmapSize), color);
            textData.emplace_back(glyphPos + glm::vec2(0, float(fontChar.size.y) * scaleFactor),
                                  glm::vec2(fontChar.pos.x, fontChar.pos.y + fontChar.size.y)/glm::vec2(this->data.bitmapSize), color);
            textData.emplace_back(glyphPos, glm::vec2(fontChar.pos)/glm::vec2(this->data.bitmapSize), color);

            pos.x += float(fontChar.advance) * scaleFactor;
        }

        pos.y += float(this->data.lineHeight) * scaleFactor;
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
    return 0;
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
    return float(fontSize)/float(this->data.size);
}