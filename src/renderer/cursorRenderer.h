//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSORRENDERER_H
#define BUILDIT_CURSORRENDERER_H


#include <GL/glew.h>
#include "shader.h"

class CursorRenderer {
private:
    GLuint vAO;
public:
    void init();
    void draw(Shader* shader);
};


#endif //BUILDIT_CURSORRENDERER_H
