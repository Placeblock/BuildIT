//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSOR_H
#define BUILDIT_CURSOR_H

#include "glm/vec2.hpp"
#include "graphics/types.h"

class Camera;

class Cursor {
public:
    intVec2 hoveringCell;
    glm::vec2 pos;
    void update(glm::vec2 mousePos, Camera camera);
};


#endif //BUILDIT_CURSOR_H
