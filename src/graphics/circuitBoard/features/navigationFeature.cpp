//
// Created by felix on 8/31/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "navigationFeature.h"

NavigationFeature::NavigationFeature(Camera *camera) : camera(camera) {

}

void NavigationFeature::onMouseMove(glm::vec2 relPos, glm::vec2 delta) {
    if (this->navigating) {
        const glm::vec2 moveDelta = glm::vec2(delta)*this->camera->getZoomScalar();
        this->camera->target -= moveDelta;
        Subject<BoardMoveEvent>::notify(BoardMoveEvent(moveDelta));
    }
}

void NavigationFeature::onScroll(glm::vec2 relPos, glm::vec2 offset) {
    glm::vec2 worldMousePos = this->camera->screenToWorld(relPos);
    this->camera->target = worldMousePos;
    this->camera->offset = -relPos;
    this->camera->zoom+= 0.1f*float(offset.y)*this->camera->zoom;
    Subject<BoardZoomEvent>::notify(BoardZoomEvent(this->camera->zoom));
}

void NavigationFeature::onMouseAction(glm::vec2 relPos, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        this->navigating = !(mods & GLFW_MOD_SHIFT) && action == GLFW_PRESS;
    }
}
