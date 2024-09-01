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
    glm::vec2 pos;
public:
    explicit Positionable(glm::vec2 pos);
    [[nodiscard]] glm::vec2 getPos() const;
    bool intersects(glm::vec2 pos);
    virtual ~Positionable() = default;
};

#endif //BUILDIT_POSITIONABLE_H
