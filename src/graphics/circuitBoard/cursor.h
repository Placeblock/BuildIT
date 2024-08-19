//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSOR_H
#define BUILDIT_CURSOR_H

#include "glm/glm.hpp"
#include "graphics/graphics.h"

class Cursor {
public:
    intVec2 hoveringCell;
    glm::vec2 pos;
    void update(glm::vec2 mousePos, Camera camera);
};


#endif //BUILDIT_CURSOR_H
