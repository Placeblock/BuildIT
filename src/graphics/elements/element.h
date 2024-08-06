//
// Created by felix on 8/6/24.
//

#ifndef BUILDIT_ELEMENT_H
#define BUILDIT_ELEMENT_H


#include "glm/vec2.hpp"

class Element {
public:
    glm::vec2 cell;
    glm::vec2 size;
    bool selected = false;
    bool hovered = false;

    virtual void onMouseOver() = 0;
    virtual void onMouseOut() = 0;
    virtual void onSelect() = 0;
    virtual void onDeselect() = 0;
    virtual void onDragStart() = 0;
    virtual void onDragUpdate(glm::vec2 oldPos, glm::vec2 newPos) = 0;
    virtual void onDragEnd(glm::vec2 pos) = 0;
    virtual void move(glm::vec2 newPos) = 0;
};


#endif //BUILDIT_ELEMENT_H
