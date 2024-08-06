//
// Created by felix on 8/6/24.
//

#ifndef BUILDIT_VERTEXRENDERER_H
#define BUILDIT_VERTEXRENDERER_H


#include <GL/glew.h>
#include <vector>
#include "graphics/shader.h"

class VertexRenderer {
public:
    void init();
    void render(Shader* shader);
    void updateVertices(std::vector<glm::vec2>* data);
    void updateVertex(int index, glm::vec2 newPos);
private:
    unsigned long count;
    GLuint vAO;
    GLuint vBOs[2];
};


#endif //BUILDIT_VERTEXRENDERER_H
