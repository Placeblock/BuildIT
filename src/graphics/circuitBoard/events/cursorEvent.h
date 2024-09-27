//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_CURSOREVENT_H
#define BUILDIT_CURSOREVENT_H

#include "glm/vec2.hpp"

struct CursorEvent {
    glm::vec2 delta;
    glm::vec2 newPos;
};

#endif //BUILDIT_CURSOREVENT_H
