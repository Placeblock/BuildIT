//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_WIDGET_H
#define BUILDIT_WIDGET_H


#include <list>
#include <memory>
#include "graphics/types.h"
#include "glm/vec2.hpp"

namespace GUI {

    enum class Action {};

    class Element;

    class GUI {
    private:
        std::unique_ptr<Element> root;
    };

    class Element {
    protected:
        GUI* gui;

        std::list<std::unique_ptr<Element>> children;

        void setBufferSize(uint newBufferSize); // Calls updateBufferSizeRecursive
    private:
        uint bufferIndex = 0;
        uint bufferSize = 0;
        uint childrenBufferSize;
        [[nodiscard]] uint getRequiredBufferSpace() const {
            return this->bufferSize + this->childrenBufferSize;
        }

        Element* parent;

        void updateBufferSizeRecursive(Element* widget, int delta);
        void moveBufferIndexRecursive(int delta);
    public:
        Element(GUI* gui, uintVec2 size, Element* parent = nullptr) : gui(gui), size(size), parent(parent) {};

        uintVec2 size;

        virtual void addChild(std::unique_ptr<Element>& child);
        virtual void removeChild(Element* child);

        void setBufferIndex(uint index);
        virtual uint calcBufferSize() = 0;

        virtual void onMouseOver(uintVec2 relPos) = 0;
        virtual void onMouseOut(uintVec2 lastInPos) = 0;
        virtual void onMouseMove(uintVec2 relPos) = 0;
        virtual void onMouseAction(uintVec2 relPos, int button, int mouseAction) = 0;

        virtual void render(uintVec2 pos, std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> &colors, std::list<uint> &texture) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_WIDGET_H
