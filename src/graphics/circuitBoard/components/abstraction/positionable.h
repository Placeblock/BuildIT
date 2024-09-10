//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_POSITIONABLE_H
#define BUILDIT_POSITIONABLE_H

#include "glm/vec2.hpp"
#include "graphics/circuitBoard/components/visitor.h"
#include "boundingBox.h"

class Movable;

class Positionable : public Boundable {
    friend class Movable; // Movable is the only class that can modify the position!

private:
    glm::vec2 pos{};
public:
    explicit Positionable(glm::vec2 pos, glm::vec2 size);

    [[nodiscard]] glm::vec2 getPos() const;

    static BoundingBox calcBoundingBox(glm::vec2 pos, glm::vec2 size);

    virtual ~Positionable() = default;
};

#endif //BUILDIT_POSITIONABLE_H
