//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Graphics {
public:
    void init();
    GLFWwindow* createWindow();
    glm::mat4 projectionMat = glm::ortho(0.0f, initScreenSize.x, initScreenSize.y, 0.0f, -1.0f, 1.0f);
    Shader* shader = nullptr;
private:
    const glm::vec2 initScreenSize = glm::vec2(640, 480);
};


#endif //BUILDIT_GRAPHICS_H
