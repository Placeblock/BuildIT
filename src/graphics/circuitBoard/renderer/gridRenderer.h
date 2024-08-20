//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_GRIDRENDERER_H
#define BUILDIT_GRIDRENDERER_H

#include <GL/glew.h>
#include "graphics/data/program.h"

class GridRenderer {
private:
    GLuint vAO;
public:
    GridRenderer();
    void render(Program* shader);
};


#endif //BUILDIT_GRIDRENDERER_H
