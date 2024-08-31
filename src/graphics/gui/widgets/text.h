//
// Created by felix on 8/15/24.
//

#ifndef BUILDIT_TEXT_H
#define BUILDIT_TEXT_H


#include "graphics/gui/element.h"
#include "graphics/font/fontMetrics.h"
#include <string>
#include <vector>
#include "graphics/types.h"

namespace GUI {
    class Text : public Element {
    private:
        Color color;
        std::string text;
        Alignment alignment;
        unsigned int fontSize;
        unsigned int vertexCount;
    public:
        Text(View *view, uintVec2 size, const std::string& text, Alignment alignment,
             Color color, unsigned int fontSize);

        void generateBuffer(std::vector<glm::vec2> &vertices, std::vector<glm::vec2> &texCoords, std::vector<Color> &colors,
                    std::vector<unsigned int> &textures) override;

        [[nodiscard]] unsigned int calcBufferSize() const override;

        void updatePos(uintVec2 newPos) override;
    };
}


#endif //BUILDIT_TEXT_H
