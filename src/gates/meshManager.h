//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_MESHMANAGER_H
#define BUILDIT_MESHMANAGER_H


#include <GL/glew.h>
#include <vector>
#include "shader.h"

class MeshManager {
public:
    void init();
    void render(Shader* shader);
private:
    GLuint vAO;
    GLuint vBOs[2]; // vertex & color
    //int indices[3][3];
    std::vector<int> counts;
};


#endif //BUILDIT_MESHMANAGER_H
