//
// Created by felix on 8/15/24.
//

#ifndef BUILDIT_TEXT_H
#define BUILDIT_TEXT_H


#include "graphics/gui/widget.h"
#include "graphics/font/fontMetrics.h"

namespace GUI {
    class Text : public Element {
    private:
        FontMetrics *metrics;
        Font *font;
        Color color;
        std::string text;
        Alignment alignment;
        uint fontSize;
        uint vertexCount;
    public:
        Text(View *view, uintVec2 size, FontMetrics *metrics, Font *font, const std::string& text, Alignment alignment,
             Color color, uint fontSize, Element* parent = nullptr);

        void onMouseOver(uintVec2 relPos) override {};

        void onMouseOut(uintVec2 lastInPos) override {};

        void onMouseMove(uintVec2 relPos) override {};

        void onMouseAction(uintVec2 relPos, int button, int mouseAction) override {};

        void render(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                    std::vector<uint> &textures) override;

        [[nodiscard]] uint calcBufferSize() const override;
    };
}


#endif //BUILDIT_TEXT_H
