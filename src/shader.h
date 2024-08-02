//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_SHADER_H
#define BUILDIT_SHADER_H

#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Shader {
    unsigned int id;
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char *geometryPath);
    void use();
    void setMat4(const std::string &name, glm::mat4 mat, bool use);
};


#endif //BUILDIT_SHADER_H
