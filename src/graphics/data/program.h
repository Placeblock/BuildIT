//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_PROGRAM_H
#define BUILDIT_PROGRAM_H

#include <string>
#include <unordered_map>
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "graphics/types.h"

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
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setColor(const std::string &name, Color value) const;
    void setFloat(const std::string &name, float value) const;
};


#endif //BUILDIT_PROGRAM_H
