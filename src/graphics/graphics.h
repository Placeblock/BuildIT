//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "shader.h"
#include "camera.h"
#include "interaction.h"
#include "simulation/simulation.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Graphics {
public:
    explicit Graphics(Sim::Simulation* simulation) : simulation(simulation) {};
    void init();
    static GLFWwindow* createWindow();
    GLFWwindow* window{};
    Camera camera;
    Shader* wireProgram = nullptr;
    Shader* vertexProgram = nullptr;
    Shader* gridProgram = nullptr;
    Shader* instancedProgram = nullptr;
    Shader* pinProgram = nullptr;
    Interaction* interaction{};
    void updateShaderUniforms();
    [[nodiscard]] glm::vec2 getMousePos() const;

    Sim::Simulation* simulation;
};


#endif //BUILDIT_GRAPHICS_H
