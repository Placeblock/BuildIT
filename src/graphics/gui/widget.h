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
#include "graphics/programs.h"
#include "graphics/font/fontMetrics.h"

namespace GUI {

    enum class Action {};

    class Element;

    class View {
    public:
        explicit View(Programs *programs);
        std::unique_ptr<Element> root;

        Font font;
        FontMetrics fontMetrics;

        void regenerateBuffers();
        void render(Program* program);

        void updateVertices(Element*, const std::vector<float>& vertices);
        void updateColors(Element*, const std::vector<unsigned char>& colors);
    private:
        Programs *programs;
        Camera camera{};

        uint vAO;
        uint vBOs[3];
        std::vector<uint> textures;

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

        Element* parent = nullptr;

        uintVec2 size;
        uintVec2 relPos = uintVec2{};
        uintVec2 absPos = uintVec2{};

        void updateBufferSizeRecursive(Element* widget, int delta);
        void moveBufferIndexRecursive(int delta);
    public:
        Element(View* view, uintVec2 size) : view(view), size(size) {};

        virtual void addChild(std::unique_ptr<Element>& child);
        virtual void removeChild(Element* child);

        virtual void updateSize(uintVec2 newSize);
        uintVec2 getSize() {return this->size;};
        virtual void onParentUpdateSize() {};
        virtual void onChildUpdateSize(Element* child) {};

        virtual void updatePos(uintVec2 newRelPos);
        uintVec2 getRelPos() {return this->relPos;};
        uintVec2 getAbsPos() {return this->absPos;};

        uint getBufferIndex() {return this->bufferIndex;};
        void setBufferIndex(uint index);
        [[nodiscard]] virtual uint calcBufferSize() const = 0;
        [[nodiscard]] uint getRequiredBufferSpace() const {
            return this->calcBufferSize() + this->childrenBufferSize;
        }

        virtual void onMouseOver(uintVec2 relPos) {};
        virtual void onMouseOut(uintVec2 lastInPos) {};
        virtual void onMouseMove(uintVec2 relPos, uintVec2 delta) {};
        virtual void onMouseAction(uintVec2 relPos, int button, int mouseAction) {};
        virtual void onScroll(uintVec2 relPos, glm::vec2 offset) {};
        virtual void onKeyAction(uintVec2 relPos, int key, int scanCode, int keyAction, int mods) {};

        virtual void render();
        bool rendered = false;
        virtual void generateBuffer(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_WIDGET_H
