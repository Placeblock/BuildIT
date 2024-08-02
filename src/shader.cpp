//
// Created by felix on 8/1/24.
//

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        if (geometryPath != nullptr) {
            gShaderFile.open(geometryPath);
        }
        std::stringstream vShaderStream, fShaderStream, gShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        if (geometryPath != nullptr) {
            gShaderStream << gShaderFile.rdbuf();
        }

        vShaderFile.close();
        fShaderFile.close();
        if (geometryPath != nullptr) {
            gShaderFile.close();
        }

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != nullptr) {
            geometryCode = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure& e) {
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

    if (geometryPath != nullptr) {
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
    if (geometryPath != nullptr) {
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
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }
}

void Shader::use() {
    glUseProgram(this->id);
}

void Shader::setMat4(const std::string &name, glm::mat4 mat, bool use) {
    if (use) this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setFloat(const std::string &name, float value, bool use) {
    if (use) this->use();
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, glm::vec2 value, bool use) {
    if (use) this->use();
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y);
}
