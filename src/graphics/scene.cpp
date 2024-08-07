//
// Created by felix on 8/7/24.
//

#include "scene.h"

void Scene::render() {
    cursorRenderer.update(cursor.cursorPos);

    gridRenderer.render(this->programs->gridProgram);
    nodes.pinRenderer.render(this->programs->pinProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    wiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    cursorRenderer.render(this->programs->vertexProgram);
}

Scene::Scene(Programs *programs) : programs(programs), interaction(Interaction(this)) {
}

void Scene::use() {
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateZoomUniforms(this->size, this->camera);
}

void Scene::updateSize(glm::vec2 size) {
    this->size = size;
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateProjectionUniforms(this->size, this->camera);
}

void Scene::updateCursor(glm::vec2 abs, glm::vec2 delta) {
    this->cursor.update(abs, this->camera);
}


// INTERACTION

void Interaction::update(glm::vec2 mousePos, glm::vec2 cursorPos, bool shiftClick) {

}

void Interaction::handleLeftClick() {

}

void Interaction::handleRightClick() {

}

void Interaction::regenerateVisData() {

}

void Interaction::renderVis(Program *wireShader, Program *vertexShader) {

}
