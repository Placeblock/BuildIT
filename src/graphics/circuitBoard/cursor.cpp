//
// Created by felix on 8/2/24.
//

#include "cursor.h"
#include "graphics/data/camera.h"

void Cursor::update(const glm::vec2 mousePos, const Camera& camera, const float timeStep) {
    glm::vec2 gridMousePos = camera.screenToWorld(mousePos) / 32.0f;
    const intVec2 roundedGridMousePos = round(gridMousePos);
    const auto deltaNearestCell = glm::vec2(gridMousePos.x - float(roundedGridMousePos.x), gridMousePos.y - float(roundedGridMousePos.y));
    if (length(deltaNearestCell) < 0.4 || length(glm::vec2(this->hoveringCell)-gridMousePos) > 1.5) {
        this->hoveringCell = roundedGridMousePos;
    }
    const glm::vec2 deltaHoveringCell = gridMousePos-glm::vec2(this->hoveringCell);
    gridMousePos = glm::vec2(this->hoveringCell)*32.0f + deltaHoveringCell * 5.0f;
    this->pos += (gridMousePos-this->pos) * std::min(timeStep * 0.03f, 1.0f);
}
