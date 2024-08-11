//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_FONTRENDERER_H
#define BUILDIT_FONTRENDERER_H


#include <GL/glew.h>

#include <utility>
#include <set>
#include "graphics/data/program.h"
#include "fontMetrics.h"

struct RenderedText {
    uint offset;
    const uint size;

    bool operator< (const RenderedText &right) const {
        return offset < right.offset;
    }
};

class FontRenderer {
private:
    GLuint vAO;
    GLuint vBOs[2];
    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::set<RenderedText> renderedTexts;

    FontMetrics metrics;
public:
    FontLoader loader;
    FontRenderer(FontMetrics metrics, FontLoader loader);
    void render(Program* program);
    RenderedText addText(const std::string& text, Alignment alignment, glm::vec2 pos);
    void removeText(RenderedText data);
    void updateBuffers();
};


#endif //BUILDIT_FONTRENDERER_H
