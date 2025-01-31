//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_FONTMETRICS_H
#define BUILDIT_FONTMETRICS_H


#include <utility>
#include <string>
#include <vector>
#include "glm/vec2.hpp"
#include "fontDataLoader.h"

enum class Alignment {LEFT, CENTER, RIGHT};

struct CharVertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
    Color color;
};

class FontMetrics {
public:
    FontData data;
    explicit FontMetrics(FontData data) : data(std::move(data)) {};

    static glm::vec2 getGlyphPos(intVec2 origin, Char cChar, float scaleFactor);
    std::vector<CharVertex> generateTextData(const std::string& text, Alignment alignment, glm::vec2 pos, unsigned int fontSize, Color color);
    std::vector<float> calculateTextWidth(const std::string& text, float scaleFactor);
    float calculateMaxTextWidth(const std::string& text, float scaleFactor);
    static std::vector<std::string> splitLines(const std::string& text);
    static float calculateAlignmentDelta(float textWidth, Alignment alignment);
    [[nodiscard]] float getScaleFactor(unsigned int fontSize) const;
};


#endif //BUILDIT_FONTMETRICS_H
