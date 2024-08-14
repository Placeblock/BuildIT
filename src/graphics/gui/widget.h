//
// Created by felix on 8/14/24.
//

#ifndef BUILDIT_WIDGET_H
#define BUILDIT_WIDGET_H


#include <list>
#include <memory>
#include "glm/vec2.hpp"

class Widget {
    glm::vec2 size;
    uint bufferIndex;
    uint bufferSize;

    Widget* parent;
    std::list<std::unique_ptr<Widget>> children;

    void setBufferSize(uint newBufferSize); // Calls updateBufferSizeRecursive
    void updateBufferSizeRecursive(Widget* widget, int delta);
    void moveBufferIndexRecursive(int delta);
public:
    ~Widget() {
        for (const auto &child: this->children) {
            child->parent = nullptr;
        }
    }
};


#endif //BUILDIT_WIDGET_H
