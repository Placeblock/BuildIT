//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"
#include "positionable.h"

struct BoundingBox {
    glm::vec2 start;
    glm::vec2 size;
};

class Movable : public Positionable {
public:
    explicit Movable(glm::vec2 pos);
    virtual void move(glm::vec2 newPos);
    ~Movable() override = default;
};

#endif //BUILDIT_MOVABLE_H
