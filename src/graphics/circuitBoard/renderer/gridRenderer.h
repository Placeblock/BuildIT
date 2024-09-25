//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_GRIDRENDERER_H
#define BUILDIT_GRIDRENDERER_H

#include <GL/glew.h>

class Program;

class GridRenderer {
    GLuint vAO = 0;
public:
    GridRenderer();
    void render(const Program* shader) const;
};


#endif //BUILDIT_GRIDRENDERER_H
