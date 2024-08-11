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
};

class FontMetrics {
private:
    FontData data;
public:
    explicit FontMetrics(FontData data) : data(std::move(data)) {};

    glm::vec2 getGlyphPos(intVec2 origin, Char cChar);
    TextData generateTextData(const std::string& text, Alignment alignment, intVec2 pos);
    uint calculateTextWidth(const std::string& text);
    static int calculateAlignmentDelta(uint textWidth, Alignment alignment);
};


#endif //BUILDIT_FONTMETRICS_H
