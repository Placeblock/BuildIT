//
// Created by felix on 8/2/24.
//

#include "cursor.h"

void Cursor::update(glm::vec2 mousePos, Camera camera) {
    glm::vec2 gridMousePos = camera.screenToWorld(mousePos) / 32.0f;
    glm::vec2 roundedGridMousePos = glm::round(gridMousePos);
    glm::vec2 deltaNearestCell = gridMousePos - roundedGridMousePos;
    if (glm::length(deltaNearestCell) < 0.4 || glm::length(this->hoveringCell-gridMousePos) > 1.5) {
        this->hoveringCell = roundedGridMousePos;
    }
    glm::vec2 deltaHoveringCell = gridMousePos - this->hoveringCell;
    gridMousePos = this->hoveringCell * 32.0f + deltaHoveringCell * 15.0f;
    this->pos += (gridMousePos-this->pos)*0.5f;
}
