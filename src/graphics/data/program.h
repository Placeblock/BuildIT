//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_PROGRAM_H
#define BUILDIT_PROGRAM_H

#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <unordered_map>

class Program {
private:
    unsigned int id;
    static std::unordered_map<std::string, std::string> shaders;
public:
    Program(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
    static std::string loadFile(const std::string& path);
    void use() const;
    void setMat4(const std::string &name, glm::mat4 mat) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setFloat(const std::string &name, float value) const;
};


#endif //BUILDIT_PROGRAM_H
