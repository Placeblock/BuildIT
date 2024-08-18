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

        uint calcBufferSize() const override;

        void onMouseOver(uintVec2 relPos) override {};
        void onMouseOut(uintVec2 lastInPos) override {};
        void onMouseMove(uintVec2 relPos) override {};
        void onMouseAction(uintVec2 relPos, int button, int mouseAction) override {};

        void render(uintVec2 pos, std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) override;

        std::vector<float> generateQuadVertices(uintVec2 pos);
    };
}

#endif //BUILDIT_IMAGE_H
