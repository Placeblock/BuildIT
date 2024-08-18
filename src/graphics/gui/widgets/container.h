//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_CONTAINER_H
#define BUILDIT_CONTAINER_H


#include "graphics/gui/widget.h"
#include <functional>

namespace GUI {
    class Container : public Element {
    protected:
        std::vector<uintVec2> childPositions;

        virtual std::vector<uintVec2> calcChildPositions() = 0;

    public:
        Container(View *gui, uintVec2 size, Element* parent = nullptr) : Element(gui, size, parent) {};

        uint calcBufferSize() const override;

        void addChild(std::unique_ptr<Element> &child) override;

        void removeChild(Element *child) override;

        void
        checkChildBounds(uintVec2 relPos, const std::function<void(std::unique_ptr<Element> &, intVec2)> &callback);

        void onMouseOver(uintVec2 relPos) override;

        void onMouseMove(uintVec2 relPos) override;

        void onMouseOut(uintVec2 lastInPos) override;

        void onMouseAction(uintVec2 relPos, int button, int mouseAction) override;

        void render(uintVec2 pos, std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                    std::vector<uint> &texture) override;
    };
}


#endif //BUILDIT_CONTAINER_H
