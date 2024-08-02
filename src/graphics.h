//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "shader.h"
#include "camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Graphics {
public:
    void init();
    static GLFWwindow* createWindow();
    GLFWwindow* window;
    Camera camera;
    Shader* lineProgram = nullptr;
    Shader* lineJointsProgram = nullptr;
    Shader* gridProgram = nullptr;
    Shader* cursorProgram = nullptr;
    void updateShaderUniforms();
    [[nodiscard]] glm::vec2 getMousePos() const;
private:
    const glm::vec2 initScreenSize = glm::vec2(640, 480);
};


#endif //BUILDIT_GRAPHICS_H
