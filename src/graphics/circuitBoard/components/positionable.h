//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_POSITIONABLE_H
#define BUILDIT_POSITIONABLE_H

#include "glm/vec2.hpp"

class Positionable; // We have to forward declare because of circ dependency

#include "movable.h"

struct BoundingBox {
    glm::vec2 start;
    glm::vec2 size;
};

class Positionable {
    friend class Movable; // Movable is the only class that can modify the position!

private:
    BoundingBox boundingBox{};
    glm::vec2 pos{};
public:
    Positionable() = default;
    explicit Positionable(glm::vec2 pos);

    [[nodiscard]] glm::vec2 getPos() const;
    bool intersects(glm::vec2 pos);
    virtual ~Positionable() = default;
};

#endif //BUILDIT_POSITIONABLE_H
