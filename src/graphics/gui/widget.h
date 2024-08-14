//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_WIDGET_H
#define BUILDIT_WIDGET_H


#include <list>
#include <memory>
#include "glm/vec2.hpp"
#include "graphics/types.h"

enum class Action {};

class Widget;

class GUI {
private:
    std::unique_ptr<Widget> root;
};

class Widget {
private:
    GUI* gui;

    intVec2 size;
    uint bufferIndex = 0;
    uint bufferSize = 0;
    uint childrenBufferSize;
    [[nodiscard]] uint getRequiredBufferSpace() const {
        return this->bufferSize + this->childrenBufferSize;
    }

    Widget* parent;
    std::list<std::unique_ptr<Widget>> children;

    void setBufferSize(uint newBufferSize); // Calls updateBufferSizeRecursive
    void updateBufferSizeRecursive(Widget* widget, int delta);
    void moveBufferIndexRecursive(int delta);

    void addChild(std::unique_ptr<Widget>& child);
    void removeChild(Widget* child);
public:
    Widget(GUI* gui, intVec2 size, Widget* parent = nullptr) : gui(gui), size(size), parent(parent) {};

    void setBufferIndex(uint index);

    virtual uint calcBufferSize() = 0;
    virtual void onMouseOver() = 0;
    virtual void onMouseOut() = 0;
    virtual void onMouseAction(intVec2 relPos, int button, int mouseAction) = 0;
    virtual void render(std::list<float>& vertices, std::list<float>& texCoords, std::list<Color> colors, std::list<uint> texture) = 0;

    virtual ~Widget() {
        for (const auto &child: this->children) {
            child->parent = nullptr;
        }
    }
};


#endif //BUILDIT_WIDGET_H
