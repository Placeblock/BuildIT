//
// Created by felix on 8/1/24.
//

#include "program.h"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "glm/gtc/type_ptr.hpp"

std::unordered_map<std::string, std::string> Program::shaders = {};

Program::Program(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        if (Program::shaders.contains(vertexPath)) {
            vertexCode = Program::shaders[vertexPath];
        } else {
            vertexCode = Program::loadFile(vertexPath);
            Program::shaders[vertexPath] = vertexCode;
        }
        if (Program::shaders.contains(fragmentPath)) {
            fragmentCode = Program::shaders[fragmentPath];
        } else {
            fragmentCode = Program::loadFile(fragmentPath);
            Program::shaders[fragmentPath] = fragmentCode;
        }
        if (!geometryPath.empty()) {
            if (Program::shaders.contains(geometryPath)) {
                geometryCode = Program::shaders[geometryPath];
            } else {
                geometryCode = Program::loadFile(geometryPath);
                Program::shaders[geometryPath] = geometryCode;
            }
        }
    } catch(std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    if (!geometryPath.empty()) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);

        glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex);
    if (!geometryPath.empty()) {
        glAttachShader(this->id, geometry);
    }
    glAttachShader(this->id, fragment);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (!geometryPath.empty()) {
        glDeleteShader(geometry);
    }
}

void Program::use() const {
    glUseProgram(this->id);
}

void Program::setMat4(const std::string &name, glm::mat4 mat) const {
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setFloat(const std::string &name, float value) const {
    this->use();
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Program::setVec2(const std::string &name, glm::vec2 value) const {
    this->use();
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y);
}

void Program::setVec3(const std::string &name, glm::vec3 value) const {
    this->use();
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z);
}

void Program::setVec4(const std::string &name, glm::vec4 value) const {
    this->use();
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Program::setColor(const std::string &name, Color value) const {
    this->setVec4(name, glm::vec4(value)/255.0f);
}



std::string Program::loadFile(const std::string& path) {
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    code = stream.str();
    return code;
}