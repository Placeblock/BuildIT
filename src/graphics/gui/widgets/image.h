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
        bool fillParent;
    public:
        Image(View *view, uintVec2 size, uint texture, Element* parent = nullptr, bool fillParent = false);

        [[nodiscard]] uint calcBufferSize() const override;

        void onMouseOver(uintVec2 relPos) override {};
        void onMouseOut(uintVec2 lastInPos) override {};
        void onMouseMove(uintVec2 relPos) override {};
        void onMouseAction(uintVec2 relPos, int button, int mouseAction) override {};

        void render(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) override;

        std::vector<float> generateQuadVertices();

        void onParentUpdateSize() override;
        void updateSize(uintVec2 newSize) override;
    };
}

#endif //BUILDIT_IMAGE_H
