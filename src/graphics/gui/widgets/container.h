//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_CONTAINER_H
#define BUILDIT_CONTAINER_H


#include "graphics/gui/element.h"
#include <list>
#include <memory>
#include <vector>
#include <functional>
#include "graphics/types.h"

namespace GUI {
    class Container : public Element {
    protected:
        virtual uintVec2 calcChildPosition(std::_List_iterator<std::unique_ptr<Element>> iter) = 0;

    public:
        Container(View *view, uintVec2 size) : Element(view, size) {};

        [[nodiscard]] uint calcBufferSize() const override;

        void addChild(std::unique_ptr<Element> &child) override;
        void removeChild(Element *child) override;

        void checkChildBounds(glm::vec2 relPos, const std::function<void(std::unique_ptr<Element> &, uintVec2)> &callback);

        void onMouseOver(glm::vec2 relPos) override;
        void onMouseMove(glm::vec2 relPos, glm::vec2 delta) override;
        void onMouseOut() override;
        void onMouseAction(glm::vec2 relPos, int button, int mouseAction) override;
        void onScroll(glm::vec2 relPos, glm::vec2 offset) override;
        void onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) override;
        void onChar(glm::vec2 relPos, unsigned char c) override;

        void generateBuffer(std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<unsigned char> &colors,
                    std::vector<uint> &texture) override;

        void updatePos(uintVec2 newRelPos) override;
        void onChildUpdateSize(Element* child) override;
    };
}


#endif //BUILDIT_CONTAINER_H
