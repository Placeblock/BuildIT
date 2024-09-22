//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_BOUNDINGBOX_H
#define BUILDIT_BOUNDINGBOX_H


#include "glm/vec2.hpp"

struct BoundingBox {
    glm::vec2 start;
    glm::vec2 size;

    [[nodiscard]] glm::vec2 clamp(glm::vec2 point) const;
    [[nodiscard]] glm::vec2 getCenter() const;

    bool contains(glm::vec2 point) const;
};

BoundingBox calcBoundingBox(glm::vec2 pos, glm::vec2 size);

#endif //BUILDIT_BOUNDINGBOX_H
