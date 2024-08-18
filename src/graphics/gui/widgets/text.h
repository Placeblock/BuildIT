//
// Created by felix on 8/15/24.
//

#ifndef BUILDIT_TEXT_H
#define BUILDIT_TEXT_H


#include "graphics/gui/widget.h"
#include "graphics/font/fontMetrics.h"

class Text : public Element {
private:
    FontMetrics* metrics;
    Font* font;
    Color color;
    std::string text;
    Alignment alignment;
    uint fontSize;
public:
    Text(GUI* gui, uintVec2 size, FontMetrics* metrics, Font* font, std::string text, Alignment alignment, Color color, uint fontSize);

    void onMouseOver(uintVec2 relPos) override {};
    void onMouseOut(uintVec2 lastInPos) override {};
    void onMouseMove(uintVec2 relPos) override {};
    void onMouseAction(uintVec2 relPos, int button, int mouseAction) override {};

    void render(uintVec2 pos, std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> &colors, std::list<uint> &texture) override;
};


#endif //BUILDIT_TEXT_H
