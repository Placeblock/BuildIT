//
// Created by felix on 8/2/24.
//

#include "cursor.h"
#include "graphics/data/camera.h"

void Cursor::update(glm::vec2 mousePos, Camera camera) {
    glm::vec2 gridMousePos = camera.screenToWorld(mousePos) / 32.0f;
    intVec2 roundedGridMousePos = glm::round(gridMousePos);
    glm::vec2 deltaNearestCell = glm::vec2(gridMousePos.x - float(roundedGridMousePos.x), gridMousePos.y - float(roundedGridMousePos.y));
    if (glm::length(deltaNearestCell) < 0.4 || glm::length(glm::vec2(this->hoveringCell)-gridMousePos) > 1.5) {
        this->hoveringCell = roundedGridMousePos;
    }
    glm::vec2 deltaHoveringCell = gridMousePos-glm::vec2(this->hoveringCell);
    gridMousePos = glm::vec2(this->hoveringCell)*32.0f + deltaHoveringCell * 5.0f;
    this->pos += (gridMousePos-this->pos)*0.5f;
}
