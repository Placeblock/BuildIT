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
        virtual uintVec2 calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) = 0;

    public:
        Container(View *view, uintVec2 size, Element* parent = nullptr) : Element(view, size, parent) {};

        [[nodiscard]] uint calcBufferSize() const override;

        void addChild(std::unique_ptr<Element> &child) override;
        void removeChild(Element *child) override;

        void checkChildBounds(uintVec2 relPos, const std::function<void(std::unique_ptr<Element> &, intVec2)> &callback);

        void onMouseOver(uintVec2 relPos) override;
        void onMouseMove(uintVec2 relPos) override;
        void onMouseOut(uintVec2 lastInPos) override;
        void onMouseAction(uintVec2 relPos, int button, int mouseAction) override;

        void render(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                    std::vector<uint> &texture) override;

        void updatePos(uintVec2 newPos) override;
        void onChildUpdateSize(Element* child) override;
    };
}


#endif //BUILDIT_CONTAINER_H
