//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_GRIDRENDERER_H
#define BUILDIT_GRIDRENDERER_H

#include <GL/glew.h>
#include "graphics/shader.h"

class GridRenderer {
private:
    GLuint vAO;
public:
    void init();
    void render(Shader* shader);
};


#endif //BUILDIT_GRIDRENDERER_H
