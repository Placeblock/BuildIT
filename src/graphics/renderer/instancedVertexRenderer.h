//
// Created by felix on 8/6/24.
//

#ifndef BUILDIT_INSTANCEDVERTEXRENDERER_H
#define BUILDIT_INSTANCEDVERTEXRENDERER_H


#include <GL/glew.h>
#include <vector>
#include "glm/vec2.hpp"
#include "graphics/buffer/vertexArray.h"

class Program;

class InstancedVertexRenderer {
public:
    InstancedVertexRenderer();
    void render(Program* shader);
    void updateVertices(std::vector<glm::vec2>* data);
    void updateVertex(unsigned int index, glm::vec2 newPos);
private:
    unsigned long count = 0;
    VertexArray va{};
    VertexBuffer<glm::vec2> instancedBuffer;
};


#endif //BUILDIT_INSTANCEDVERTEXRENDERER_H
