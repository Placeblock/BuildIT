//
// Created by felix on 8/2/24.
//

#include "camera.h"


glm::mat4 Camera::getProjectionMat(glm::vec2 screenSize) const {
    const glm::vec2 pos = this->getPos();
    return glm::ortho(pos.x, pos.x+screenSize.x*this->getZoomScalar(), pos.y+screenSize.y*this->getZoomScalar(), pos.y);
}

glm::vec2 Camera::screenToWorld(glm::vec2 screenPos) const {
    return this->getPos() + this->getZoomScalar()*screenPos;
}

glm::vec2 Camera::getPos() const {
    return this->target+this->offset*this->getZoomScalar();
}

float Camera::getZoomScalar() const {
    if (this->zoom == 0) return 0;
    return 1/this->zoom;
}