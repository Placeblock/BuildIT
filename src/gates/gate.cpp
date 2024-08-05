//
// Created by felix on 8/4/24.
//

#include "gate.h"

std::unordered_set<glm::vec2> Gate::calculatePinCells() {
    std::unordered_set<glm::vec2> cells;

    float iDelta = this->size.y / float(this->inputs + 1);
    for (int i = 1; i <= this->inputs; ++i) {
        int pos = round(i * iDelta);
        cells.insert(glm::vec2(this->cell.x, this->cell.y+pos));
    }
    float oDelta = this->size.y / float(this->outputs + 1);
    for (int i = 1; i <= this->outputs; ++i) {
        int pos = round(i * oDelta);
        cells.insert(glm::vec2(this->cell.x+this->size.x, this->cell.y+pos));
    }

    return cells;
}
