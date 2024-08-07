//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_PROGRAMS_H
#define BUILDIT_PROGRAMS_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics/data/program.h"
#include "graphics/data/camera.h"

class Programs {
public:
    Programs();
    Program* wireProgram = nullptr;
    Program* vertexProgram = nullptr;
    Program* gridProgram = nullptr;
    Program* instancedProgram = nullptr;
    Program* pinProgram = nullptr;
    void updateProjectionUniforms(glm::vec2 windowSize, Camera camera);
    void updateZoomUniforms(glm::vec2 windowSize, Camera camera);
};


#endif //BUILDIT_PROGRAMS_H
