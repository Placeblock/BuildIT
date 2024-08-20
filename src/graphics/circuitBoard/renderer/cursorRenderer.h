//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSORRENDERER_H
#define BUILDIT_CURSORRENDERER_H


#include <GL/glew.h>
#include "graphics/data/program.h"

class CursorRenderer {
private:
    GLuint vAO;
    GLuint vBOs[2];
public:
    CursorRenderer();
    void update(glm::vec2 cursorPos);
    void render(Program* shader);
};


#endif //BUILDIT_CURSORRENDERER_H
