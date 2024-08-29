//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_ELEMENT_H
#define BUILDIT_ELEMENT_H


#include <list>
#include <memory>
#include "graphics/types.h"
#include <vector>
#include "glm/vec2.hpp"
#include "graphics/font/fontMetrics.h"
#include "graphics/font/fontRenderer.h"
#include "graphics/data/camera.h"
#include "graphics/font/fontLoader.h"
#include "graphics/programs.h"

namespace GUI {
    enum class Action {};

    class Element;

    class View {
    public:
        explicit View(Programs *programs);
        std::unique_ptr<Element> root;

        Font font;
        FontMetrics fontMetrics;
        FontRenderer fontRenderer;

        glm::vec2 mousePos;

        Element* focused = nullptr;

        void regenerateBuffers();
        void render();

        void updateVertices(Element*, const std::vector<float>& vertices);
        void updateColors(Element*, const std::vector<unsigned char>& colors);

        void moveMouse(glm::vec2 newPos);
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

        bool mouseOver = false;

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

        virtual void onMouseOver(glm::vec2 relPos) {this->mouseOver = true;};
        virtual void onMouseOut() {this->mouseOver = false;};
        virtual void onMouseMove(glm::vec2 relPos, glm::vec2 delta) {};
        virtual void onMouseAction(glm::vec2 relPos, int button, int mouseAction) {};
        virtual void onScroll(glm::vec2 relPos, glm::vec2 offset) {};
        virtual void onKeyAction(glm::vec2 relPos, int key, int scanCode, int keyAction, int mods) {};
        virtual void onChar(glm::vec2 relPos, unsigned char c) {};

        virtual void prerender(Programs* programs);
        virtual void postrender(Programs* programs);
        bool rendered = false;
        virtual void generateBuffer(std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<unsigned char> &colors, std::vector<uint> &textures) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_ELEMENT_H
