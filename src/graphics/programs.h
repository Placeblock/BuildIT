//
// Created by felix on 8/7/24.
//

#ifndef BUILDIT_PROGRAMS_H
#define BUILDIT_PROGRAMS_H


#include <GLFW/glfw3.h>
#include "program.h"
#include "graphicData.h"

class Programs {
private:
    GraphicData* graphicData;
public:
    explicit Programs(GraphicData* data);
    Program* wireProgram = nullptr;
    Program* vertexProgram = nullptr;
    Program* gridProgram = nullptr;
    Program* instancedProgram = nullptr;
    Program* pinProgram = nullptr;
    void updateProjectionUniforms();
    void updateZoomUniforms();
};


#endif //BUILDIT_PROGRAMS_H
