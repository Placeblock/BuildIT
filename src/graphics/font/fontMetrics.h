//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_FONTMETRICS_H
#define BUILDIT_FONTMETRICS_H


#include <utility>

#include "glm/vec2.hpp"
#include "fontDataLoader.h"
#include "fontLoader.h"

enum class Alignment {LEFT, CENTER, RIGHT};

struct TextData {
    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::vector<unsigned char> colors;
};

class FontMetrics {
public:
    FontData data;
    explicit FontMetrics(FontData data) : data(std::move(data)) {};

    glm::vec2 getGlyphPos(intVec2 origin, Char cChar, float scaleFactor);
    TextData generateTextData(const std::string& text, Alignment alignment, intVec2 pos, uint fontSize, Color color);
    std::vector<float> calculateTextWidth(const std::string& text, float scaleFactor);
    float calculateMaxTextWidth(const std::string& text, float scaleFactor);
    static std::vector<std::string> splitLines(const std::string& text);
    static float calculateAlignmentDelta(float textWidth, Alignment alignment);
    float getScaleFactor(uint fontSize) const;
};


#endif //BUILDIT_FONTMETRICS_H
