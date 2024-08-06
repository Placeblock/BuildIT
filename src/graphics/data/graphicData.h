//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_GRAPHICDATA_H
#define BUILDIT_GRAPHICDATA_H


#include <GLFW/glfw3.h>
#include "camera.h"

struct GraphicData {
    GraphicData(GLFWwindow* window, Camera camera) : window(window), camera(camera) {};
    GLFWwindow* window;
    Camera camera;
};


#endif //BUILDIT_GRAPHICDATA_H
