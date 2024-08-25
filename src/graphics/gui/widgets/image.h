//
// Created by felix on 8/18/24.
//

#ifndef BUILDIT_IMAGE_H
#define BUILDIT_IMAGE_H


#include "graphics/gui/element.h"

namespace GUI {
    class Image : public Element {
    private:
        uint texture;
        bool fillParent;
    public:
        Image(View *view, uintVec2 size, uint texture, bool fillParent = false);

        [[nodiscard]] uint calcBufferSize() const override;

        void generateBuffer(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) override;

        std::vector<float> generateQuadVertices();

        void onParentUpdateSize() override;
        void updateSize(uintVec2 newSize) override;
    };
}

#endif //BUILDIT_IMAGE_H
