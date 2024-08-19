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

        void updateVertices(Element*, const std::vector<float>& vertices);
        void updateColors(Element*, const std::vector<unsigned char>& colors);
    private:
        std::vector<float> vertexBuffer;
        std::vector<float> texCoordBuffer;
        std::vector<unsigned char> colorBuffer;
    };

    class Element {
    protected:
        View* view;

        std::list<std::unique_ptr<Element>> children;
        Element* getParent() {return this->parent;};

        void setBufferSize(int delta); // Calls updateBufferSizeRecursive
    private:
        uint bufferIndex = 0;
        uint childrenBufferSize = 0;

        Element* parent;

        uintVec2 size;
        uintVec2 pos = uintVec2{};

        void updateBufferSizeRecursive(Element* widget, int delta);
        void moveBufferIndexRecursive(int delta);
    public:
        Element(View* view, uintVec2 size, Element* parent = nullptr) : view(view), size(size), parent(parent) {};

        virtual void addChild(std::unique_ptr<Element>& child);
        virtual void removeChild(Element* child);

        virtual void updateSize(uintVec2 newSize);
        uintVec2 getSize() {return this->size;};
        virtual void onParentUpdateSize() {};
        virtual void onChildUpdateSize(Element* child) {};

        virtual void updatePos(uintVec2 newPos) { this->pos = newPos;};
        uintVec2 getPos() {return this->pos;};

        uint getBufferIndex() {return this->bufferIndex;};
        void setBufferIndex(uint index);
        [[nodiscard]] virtual uint calcBufferSize() const = 0;
        [[nodiscard]] uint getRequiredBufferSpace() const {
            return this->calcBufferSize() + this->childrenBufferSize;
        }

        virtual void onMouseOver(uintVec2 relPos) {};
        virtual void onMouseOut(uintVec2 lastInPos) {};
        virtual void onMouseMove(uintVec2 relPos) {};
        virtual void onMouseAction(uintVec2 relPos, int button, int mouseAction) {};
        virtual void onScroll(uintVec2 relPos, glm::vec2 offset) {};

        virtual void render(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_WIDGET_H
