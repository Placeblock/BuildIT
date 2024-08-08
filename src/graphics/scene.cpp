//
// Created by felix on 8/7/24.
//

#include "scene.h"

void Scene::render() {
    glViewport(0,0,this->size.x, this->size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    this->programs->updateProjectionUniforms(this->size, this->camera);

    cursorRenderer.update(this->cursor.pos);

    gridRenderer.render(this->programs->gridProgram);
    nodes.pinRenderer.render(this->programs->pinProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    wiresRenderer.render(this->programs->wireProgram, this->programs->vertexProgram);

    this->programs->vertexProgram->setFloat("size", 15.0);
    cursorRenderer.render(this->programs->vertexProgram);
}

Scene::Scene(Programs *programs, vpSize size) : programs(programs), size(size) {
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::use() {
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateZoomUniforms(this->size, this->camera);
}

void Scene::onResize(vpSize newSize) {
    this->size = newSize;
    this->programs->gridProgram->setVec2("resolution", this->size);
    this->programs->updateProjectionUniforms(this->size, this->camera);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Scene::onMouseMove(glm::vec2 abs, glm::vec2 delta) {
	this->mousePos = abs;
    this->cursor.update(abs, this->camera);
    if (this->action == dragScene) {
		this->camera.target -= delta*this->camera.getZoomScalar();
        //this->programs->updateProjectionUniforms(this->size, this->camera);
    }
}

void Scene::onMouseAction(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    	this->action = dragScene;
    }
    if (action == GLFW_RELEASE) {
    	this->action = nothing;
    }
}

void Scene::onKeyAction(int key, int scanCode, int action, int mods) {
	
}

void Scene::onScroll(glm::vec2 offset) {
    glm::vec2 worldMousePos = this->camera.screenToWorld(this->mousePos);
    this->camera.target = worldMousePos;
    this->camera.offset = -this->mousePos;
    this->camera.zoom+= 0.1f*float(offset.y)*this->camera.zoom;
    this->programs->updateZoomUniforms(this->size, this->camera);
}
