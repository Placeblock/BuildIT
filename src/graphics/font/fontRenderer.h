//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_FONTRENDERER_H
#define BUILDIT_FONTRENDERER_H


#include <GL/glew.h>

#include <utility>
#include <set>
#include <memory>
#include <list>
#include "graphics/data/program.h"
#include "fontMetrics.h"
#include "fontLoader.h"
#include "graphics/buffer/vertexArray.h"

struct RenderedText {
    const std::string text;
    const Alignment alignment;
    const unsigned int fontSize;
    Color color;
    BufferSection* bufferSection;

    bool operator< (const RenderedText &right) const {
        return bufferSection->elementIndex < right.bufferSection->elementIndex;
    }
};

class FontRenderer {
private:
    VertexArray vertexArray;
    SectionedBuffer<CharVertex> vertexBuffer;

    Font font;
    FontMetrics metrics;
public:
    explicit FontRenderer(const Font& font);
    void render(Program* program);
    std::unique_ptr<RenderedText> addText(const std::string& text, Alignment alignment, glm::vec2 pos, unsigned int fontSize, Color color);
    void removeText(RenderedText *data);
    void moveText(RenderedText *data, glm::vec2 newPos);
    void updateBuffers();
};


#endif //BUILDIT_FONTRENDERER_H
