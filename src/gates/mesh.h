//
// Created by felix on 8/4/24.
//

#ifndef BUILDIT_MESH_H
#define BUILDIT_MESH_H


#include "glm/gtx/hash.hpp"
#include <GL/glew.h>
#include <vector>
#include <unordered_set>
#include "shader.h"

class Mesh {
public:
    void init(std::vector<float> vertices, std::vector<unsigned char> colors, std::vector<unsigned int> indices);
    void render(Shader* shader);
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


#endif //BUILDIT_MESH_H
