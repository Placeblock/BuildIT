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

class InstancedMeshRenderer {
public:
    InstancedMeshRenderer(std::vector<float> vertices, std::vector<unsigned char> colors, std::vector<unsigned int> indices);
    void render(Program* shader);
    void addInstance(glm::vec2 pos);
    void removeInstance(glm::vec2 pos);
    void updateInstance(glm::vec2 pos, glm::vec2 newPos, bool updateSSBO);
    void updateInstance(int index, glm::vec2 newPos, bool updateSSBO);
    void updateSSBO();
    std::vector<glm::vec2> positions;
private:
    long indexCount;
    GLuint vAO;
    GLuint vBOs[4]; // vertex & color & indices & ssbo
};


#endif //BUILDIT_INSTANCEDMESHRENDERER_H
