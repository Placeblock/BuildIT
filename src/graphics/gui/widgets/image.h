//
// Created by felix on 8/18/24.
//

#ifndef BUILDIT_IMAGE_H
#define BUILDIT_IMAGE_H

#include <vector>
#include "graphics/types.h"
#include "graphics/gui/element.h"

namespace GUI {
    class Image : public Element {
    private:
        unsigned int texture;
        bool fillParent;
    public:
        Image(View *view, uintVec2 size, unsigned int texture, bool fillParent = false);

        [[nodiscard]] unsigned int calcBufferSize() const override;

        void generateBuffer(std::vector<glm::vec2>& vertices, std::vector<glm::vec2>& texCoords,
                            std::vector<Color> &colors, std::vector<unsigned int> &textures) override;

        std::vector<glm::vec2> generateQuadVertices();

        void onParentUpdateSize() override;
        void updateSize(uintVec2 newSize) override;
    };
}

#endif //BUILDIT_IMAGE_H
