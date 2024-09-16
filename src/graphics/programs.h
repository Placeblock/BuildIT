//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_PROGRAMS_H
#define BUILDIT_PROGRAMS_H

#include "glm/vec2.hpp"

class Camera;
class Program;

class Programs {
public:
    Programs();
    Program* wireProgram = nullptr;
    Program* vertexProgram = nullptr;
    Program* gridProgram = nullptr;
    Program* instancedProgram = nullptr;
    Program* pinProgram = nullptr;
    Program* textureProgram = nullptr;
    Program* defaultProgram = nullptr;
    void updateProjectionUniforms(glm::vec2 windowSize, Camera camera);
    void updateZoomUniforms(glm::vec2 windowSize, Camera camera);
};


#endif //BUILDIT_PROGRAMS_H
