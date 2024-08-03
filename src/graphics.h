//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "shader.h"
#include "camera.h"
#include "interaction.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Graphics {
public:
    void init();
    static GLFWwindow* createWindow();
    GLFWwindow* window;
    Camera camera;
    Shader* wireProgram = nullptr;
    Shader* vertexProgram = nullptr;
    Shader* gridProgram = nullptr;
    Shader* cursorProgram = nullptr;
    Interaction* interaction;
    void updateShaderUniforms();
    [[nodiscard]] glm::vec2 getMousePos() const;
};


#endif //BUILDIT_GRAPHICS_H
