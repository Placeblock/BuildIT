//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include "program.h"
#include "camera.h"
#include "interaction.h"
#include "simulation/simulation.h"
#include "eventHandler.h"
#include "renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Graphics {
public:
    explicit Graphics(Sim::Simulation* simulation) : simulation(simulation) {};
    void init();
    EventHandler* eventHandler;
    Renderer* renderer;
    static GLFWwindow* createWindow();
    Sim::Simulation* simulation;
};


#endif //BUILDIT_GRAPHICS_H
