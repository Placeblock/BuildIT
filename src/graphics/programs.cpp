//
// Created by felix on 8/7/24.
//

#include "programs.h"

void Programs::updateProjectionUniforms() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(this->graphicData->window, &windowWidth, &windowHeight);
    glm::mat4 projectionMat = this->graphicData->camera.getProjectionMat(glm::vec2(windowWidth, windowHeight));
    this->wireProgram->setMat4("projection", projectionMat, true);
    this->vertexProgram->setMat4("projection", projectionMat, true);
    this->gridProgram->setVec2("offset", this->graphicData->camera.getPos(), true);
    this->instancedProgram->setMat4("projection", projectionMat, true);
    this->pinProgram->setMat4("projection", projectionMat, true);
}

void Programs::updateZoomUniforms() {
    this->vertexProgram->setFloat("zoom", this->graphicData->camera.zoom, true);
    this->gridProgram->setFloat("zoom", this->graphicData->camera.zoom, true);
    this->pinProgram->setFloat("zoom", this->graphicData->camera.zoom, true);
    this->updateProjectionUniforms();
}

Programs::Programs(GraphicData *data) : graphicData(data) {
    this->vertexProgram = new Program("resources/shaders/circleVertexShader.vs",
                                      "resources/shaders/pointFragmentShader.fs",
                                      "resources/shaders/pointGeometryShader.gs");
    this->wireProgram = new Program("resources/shaders/defaultVertexShader.vs",
                                    "resources/shaders/defaultFragmentShader.fs",
                                    "resources/shaders/wireGeometryShader.gs");
    this->gridProgram = new Program("resources/shaders/defaultVertexShader.vs",
                                    "resources/shaders/gridShader.fs");
    this->pinProgram = new Program("resources/shaders/offsetInstancedSingleColorCircleVertexShader.vs",
                                   "resources/shaders/pointFragmentShader.fs",
                                   "resources/shaders/pointGeometryShader.gs");
    this->instancedProgram = new Program("resources/shaders/offsetInstancedVertexShader.vs",
                                         "resources/shaders/defaultFragmentShader.fs");
    this->pinProgram->setFloat("size", 10, true);
    this->pinProgram->setVec3("color", glm::vec3(150, 150, 0), false);
}
