//
// Created by felix on 8/6/24.
//

#ifndef BUILDIT_INSTANCEDVERTEXRENDERER_H
#define BUILDIT_INSTANCEDVERTEXRENDERER_H


#include <GL/glew.h>
#include <vector>
#include "graphics/program.h"

class InstancedVertexRenderer {
public:
    void init();
    void render(Program* shader);
    void updateVertices(std::vector<glm::vec2>* data);
    void updateVertex(int index, glm::vec2 newPos);
private:
    unsigned long count;
    GLuint vAO;
    GLuint vBOs[2];
};


#endif //BUILDIT_INSTANCEDVERTEXRENDERER_H
