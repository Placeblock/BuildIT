//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_INSTANCEDMESHRENDERER_H
#define BUILDIT_INSTANCEDMESHRENDERER_H

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include "glm/gtx/hash.hpp"
#include <vector>
#include <unordered_set>
#include "graphics/data/program.h"
#include "graphics/buffer/vertexArray.h"

class InstancedMeshRenderer {
public:
    InstancedMeshRenderer(const std::vector<VertexData>& vertices, std::vector<unsigned int> indices);
    void render(Program* shader);
    void addInstance(glm::vec2 pos);
    void removeInstance(glm::vec2 pos);
    void updateInstance(glm::vec2 pos, glm::vec2 newPos);
private:
    std::vector<glm::vec2> positions;
    bool stale = false;
    long indexCount;
    VertexArray va;
    VertexBuffer<VertexData> vb;
    VertexBuffer<glm::vec2> instancedBuffer;
    GLuint indicesBuffer;

    void updateInstanceData();
    void rebufferInstanceData();
};


#endif //BUILDIT_INSTANCEDMESHRENDERER_H
