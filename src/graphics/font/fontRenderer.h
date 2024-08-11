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

struct RenderedText {
    const std::string text;
    const Alignment alignment;
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
    std::list<std::shared_ptr<RenderedText>> renderedTexts;

    FontMetrics metrics;
public:
    FontLoader loader;
    FontRenderer(FontMetrics metrics, FontLoader loader);
    void render(Program* program);
    std::shared_ptr<RenderedText> addText(const std::string& text, Alignment alignment, glm::vec2 pos);
    void removeText(const std::shared_ptr<RenderedText>& data);
    void moveText(const std::shared_ptr<RenderedText>& data, glm::vec2 newPos);
    void updateBuffers();
};


#endif //BUILDIT_FONTRENDERER_H
