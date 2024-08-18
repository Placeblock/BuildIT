//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_WIDGET_H
#define BUILDIT_WIDGET_H


#include <list>
#include <memory>
#include "graphics/types.h"
#include "glm/vec2.hpp"
#include "graphics/data/program.h"

namespace GUI {

    enum class Action {};

    class Element;

    class View {
    public:
        View();
        uint vAO;
        uint vBOs[3];
        std::list<uint> textures;
    private:
        std::unique_ptr<Element> root;

        std::list<float> vertexBuffer;
        std::list<float> texCoordBuffer;
        std::list<Color> colorBuffer;

        void regenerateBuffers();

        void render(Program* program);
    };

    class Element {
    protected:
        View* gui;

        uint bufferIndex = 0;

        std::list<std::unique_ptr<Element>> children;

        void setBufferSize(uint newBufferSize); // Calls updateBufferSizeRecursive
    private:
        uint bufferSize = 0;
        uint childrenBufferSize;

        Element* parent;

        void updateBufferSizeRecursive(Element* widget, int delta);
        void moveBufferIndexRecursive(int delta);
    public:
        Element(View* gui, uintVec2 size, Element* parent = nullptr) : gui(gui), size(size), parent(parent) {};

        uintVec2 size;

        virtual void addChild(std::unique_ptr<Element>& child);
        virtual void removeChild(Element* child);

        void setBufferIndex(uint index);
        virtual uint calcBufferSize() = 0;
        [[nodiscard]] uint getRequiredBufferSpace() const {
            return this->bufferSize + this->childrenBufferSize;
        }

        virtual void onMouseOver(uintVec2 relPos) = 0;
        virtual void onMouseOut(uintVec2 lastInPos) = 0;
        virtual void onMouseMove(uintVec2 relPos) = 0;
        virtual void onMouseAction(uintVec2 relPos, int button, int mouseAction) = 0;

        virtual void render(uintVec2 pos, std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> &colors, std::list<uint> &textures) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_WIDGET_H
