//
// Created by felix on 8/1/24.
//

#ifndef BUILDIT_GRAPHICS_H
#define BUILDIT_GRAPHICS_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics/data/program.h"
#include "graphics/data/camera.h"
#include "simulation/simulation.h"
#include "graphics/data/eventHandler.h"
#include "graphics/data/renderer.h"


/**
 * Contains all the logic for initializing glfw
 * Creates the actual application
 */
class Graphics {
public:
    explicit Graphics(Sim::Simulation* simulation) : simulation(simulation) {};
    void init();
    EventHandler* eventHandler = nullptr;
    Renderer* renderer = nullptr;
    static GLFWwindow* createWindow();
    Sim::Simulation* simulation;
};


#endif //BUILDIT_GRAPHICS_H
