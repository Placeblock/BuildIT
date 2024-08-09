//
// Created by felix on 8/7/24.
//

#include "programs.h"

void Programs::updateProjectionUniforms(glm::vec2 windowSize, Camera camera) {
    glm::mat4 projectionMat = camera.getProjectionMat(glm::vec2(windowSize.x, windowSize.y));
    this->wireProgram->setMat4("projection", projectionMat);
    this->vertexProgram->setMat4("projection", projectionMat);
    this->gridProgram->setVec2("offset", camera.getPos());
    this->instancedProgram->setMat4("projection", projectionMat);
    this->pinProgram->setMat4("projection", projectionMat);
    this->textureProgram->setMat4("projection", projectionMat);
}

void Programs::updateZoomUniforms(glm::vec2 windowSize, Camera camera) {
    this->vertexProgram->setFloat("zoom", camera.zoom);
    this->gridProgram->setFloat("zoom", camera.zoom);
    this->pinProgram->setFloat("zoom", camera.zoom);
    this->updateProjectionUniforms(windowSize, camera);
}

Programs::Programs() {
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
    this->textureProgram = new Program("resources/shaders/textureVertexShader.vs",
                                         "resources/shaders/textureFragmentShader.fs");
    this->pinProgram->setFloat("size", 10);
    this->pinProgram->setVec3("color", glm::vec3(150, 150, 0));
}
