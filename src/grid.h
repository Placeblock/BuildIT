//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_GRID_H
#define BUILDIT_GRID_H

#include <GL/glew.h>
#include "shader.h"

class Grid {
private:
    GLuint vAO;
public:
    void init();
    void draw(Shader* shader);
};


#endif //BUILDIT_GRID_H
