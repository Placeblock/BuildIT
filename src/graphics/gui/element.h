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
#include "graphics/buffer/vertexArray.h"
#include "graphics/eventHandler.h"

namespace GUI {
    class SizeAccessor {
    public:
        virtual uintVec2 getSize() = 0;
    };

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

        void updateVertices(Element*, const std::vector<glm::vec2>& vertices);
        void updateColors(Element*, const std::vector<Color>& colors);

        void moveMouse(glm::vec2 newPos);
    private:
        Programs *programs;
        Camera camera{};

        VertexArray vertexArray{};
        VertexBuffer<glm::vec2> vertexBuffer;
        VertexBuffer<glm::vec2> texBuffer;
        VertexBuffer<Color> colorBuffer;

        std::vector<uint> textures;
    };

class Element : public EventHandler, public SizeAccessor {
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
        uintVec2 getSize() override {return this->size;};
        virtual void onParentUpdateSize() {};
        virtual void onChildUpdateSize(Element* child) {};

        virtual void updatePos(uintVec2 newRelPos);
        uintVec2 getRelPos() {return this->relPos;};
        uintVec2 getAbsPos() {return this->absPos;};

        uint getBufferIndex() const {return this->bufferIndex;};
        void setBufferIndex(uint index);
        [[nodiscard]] virtual uint calcBufferSize() const = 0;
        [[nodiscard]] uint getRequiredBufferSpace() const {
            return this->calcBufferSize() + this->childrenBufferSize;
        }

        virtual void prerender(Programs* programs);
        virtual void postrender(Programs* programs);
        bool rendered = false;
        virtual void generateBuffer(std::vector<glm::vec2>& vertices, std::vector<glm::vec2>& texCoords, std::vector<Color> &colors, std::vector<uint> &textures) = 0;

        virtual ~Element() {
            for (const auto &child: this->children) {
                child->parent = nullptr;
            }
        }
    };
}


#endif //BUILDIT_ELEMENT_H
