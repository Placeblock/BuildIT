//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_CURSOR_H
#define BUILDIT_CURSOR_H


#include <GL/glew.h>
#include "shader.h"

class Cursor {
private:
    GLuint vAO;
public:
    void init();
    void draw(Shader* shader);
};


#endif //BUILDIT_CURSOR_H
