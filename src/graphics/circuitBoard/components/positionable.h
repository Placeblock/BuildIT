//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_POSITIONABLE_H
#define BUILDIT_POSITIONABLE_H

#include "glm/vec2.hpp"
#include "movable.h"

class Positionable {
private:
    BoundingBox boundingBox;
protected:
    glm::vec2 cell;
public:
    [[nodiscard]] glm::vec2 getCell() const;
    bool intersects(glm::vec2 pos);
};

#endif //BUILDIT_POSITIONABLE_H
