//
// Created by felix on 8/14/24.
//

#include "horizontalList.h"

std::vector<uintVec2> HorizontalList::calcChildPositions() {
    std::vector<uintVec2> positions;
    uint currentX = 0;
    for (const auto &child: this->children) {
        positions.emplace_back(currentX, 0);
        currentX += child->size.x;
    }
    return positions;
}
