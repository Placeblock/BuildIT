//
// Created by felix on 8/18/24.
//

#ifndef BUILDIT_IMAGE_H
#define BUILDIT_IMAGE_H


#include "graphics/gui/widget.h"

namespace GUI {
    class Image : public Element {
    private:
        uint texture;
    public:
        Image(View *gui, uintVec2 size, uint texture, Element* parent = nullptr);

        uint calcBufferSize() override;

        void onMouseOver(uintVec2 relPos) override {};
        void onMouseOut(uintVec2 lastInPos) override {};
        void onMouseMove(uintVec2 relPos) override {};
        void onMouseAction(uintVec2 relPos, int button, int mouseAction) override {};

        void render(uintVec2 pos, std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> &colors, std::list<uint> &textures) override;

        std::vector<float> generateQuadVertices(uintVec2 pos);
    };
}

#endif //BUILDIT_IMAGE_H
