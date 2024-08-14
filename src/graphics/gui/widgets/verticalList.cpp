//
// Created by felix on 8/14/24.
//

#include "verticalList.h"

std::vector<glm::vec2> VerticalList::calcChildPositions() {
    std::vector<glm::vec2> positions;
    uint currentY = 0;
    for (const auto &child: this->children) {
        positions.emplace_back(0, currentY);
        currentY += child->size.y;
    }
    return positions;
}
