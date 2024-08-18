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
        std::vector<uint> textures;
        std::unique_ptr<Element> root;

        void regenerateBuffers();
        void render(Program* program);
    private:

        std::vector<float> vertexBuffer;
        std::vector<float> texCoordBuffer;
        std::vector<unsigned char> colorBuffer;

    };

    class Element {
    protected:
        View* gui;

        uint bufferIndex = 0;

        std::list<std::unique_ptr<Element>> children;

        void setBufferSize(uint delta); // Calls updateBufferSizeRecursive
    private:
        uint childrenBufferSize = 0;

        Element* parent;

        void updateBufferSizeRecursive(Element* widget, int delta);
        void moveBufferIndexRecursive(int delta);
    public:
        Element(View* gui, uintVec2 size, Element* parent = nullptr) : gui(gui), size(size), parent(parent) {};

        uintVec2 size;

        virtual void addChild(std::unique_ptr<Element>& child);
        virtual void removeChild(Element* child);

        void setBufferIndex(uint index);
        [[nodiscard]] virtual uint calcBufferSize() const = 0;
        [[nodiscard]] uint getRequiredBufferSpace() const {
            return this->calcBufferSize() + this->childrenBufferSize;
        }

        virtual void onMouseOver(uintVec2 relPos) = 0;
        virtual void onMouseOut(uintVec2 lastInPos) = 0;
        virtual void onMouseMove(uintVec2 relPos) = 0;
        virtual void onMouseAction(uintVec2 relPos, int button, int mouseAction) = 0;

        virtual void render(uintVec2 pos, std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_WIDGET_H
