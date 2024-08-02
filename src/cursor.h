//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSOR_H
#define BUILDIT_CURSOR_H

#include "glm/glm.hpp"
#include "graphics.h"

class Cursor {
public:
    glm::vec2 hoveringCell;
    glm::vec2 cursorPos;
    void update(glm::vec2 mousePos, Camera camera);
};


#endif //BUILDIT_CURSOR_H
