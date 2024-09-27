//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_MOVEEVENT_H
#define BUILDIT_MOVEEVENT_H

#include "glm/vec2.hpp"

class Movable;

struct MoveEvent {
    Movable *movable;
    glm::vec2 delta;
    bool before = false;
};

#endif //BUILDIT_MOVEEVENT_H
