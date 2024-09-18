//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSORRENDERER_H
#define BUILDIT_CURSORRENDERER_H


#include <GL/glew.h>
#include "glm/vec2.hpp"

class Program;

class CursorRenderer {
    GLuint vAO = 0;
    GLuint vBOs[2] = {0, 0};
public:
    CursorRenderer();
    void update(glm::vec2 cursorPos) const;
    void render(const Program* shader) const;
};


#endif //BUILDIT_CURSORRENDERER_H
