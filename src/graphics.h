//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec2 target{0.0f, 0.0f};
    glm::vec2 offset{0.0f, 0.0f};
    float zoom = 1.0;
    float getZoomScalar();
    glm::vec2 getPos();
    glm::mat4 getProjectionMat(glm::vec2 screenSize);
    glm::vec2 screenToWorld(glm::vec2 screenPos);
};

class Graphics {
public:
    void init();
    GLFWwindow* createWindow();
    GLFWwindow* window;
    Camera camera;
    Shader* lineProgram = nullptr;
    Shader* lineJointsProgram = nullptr;
    Shader* gridProgram = nullptr;
    void updateShaderUniforms();
    glm::vec2 getMousePos();
private:
    const glm::vec2 initScreenSize = glm::vec2(640, 480);
};


#endif //BUILDIT_GRAPHICS_H
