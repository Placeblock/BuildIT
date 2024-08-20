//
// Created by felix on 8/15/24.
//

#ifndef BUILDIT_TEXT_H
#define BUILDIT_TEXT_H


#include "graphics/gui/element.h"
#include "graphics/font/fontMetrics.h"

namespace GUI {
    class Text : public Element {
    private:
        Color color;
        std::string text;
        Alignment alignment;
        uint fontSize;
        uint vertexCount;
    public:
        Text(View *view, uintVec2 size, const std::string& text, Alignment alignment,
             Color color, uint fontSize);

        void generateBuffer(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                    std::vector<uint> &textures) override;

        [[nodiscard]] uint calcBufferSize() const override;

        void updatePos(uintVec2 newPos) override;
    };
}


#endif //BUILDIT_TEXT_H
