//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_POSITIONABLE_H
#define BUILDIT_POSITIONABLE_H

#include "glm/vec2.hpp"

template <typename T>
class Positionable; // We have to forward declare because of circ dependency

#include "movable.h"

struct BoundingBox {
    glm::vec2 start;
    glm::vec2 size;
};

template <typename T>
class Positionable {
    friend class Movable<T>; // Movable is the only class that can modify the position!

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

template <typename T>
Positionable<T>::Positionable(glm::vec2 pos) : pos(pos) {}

template <typename T>
glm::vec2 Positionable<T>::getPos() const {
    return this->pos;
}

template <typename T>
bool Positionable<T>::intersects(glm::vec2 pos) {
    const glm::vec2 relative = pos - this->pos;
    return relative.x >= this->boundingBox.start.x && relative.y >= this->boundingBox.start.y &&
           relative.x <= this->boundingBox.start.x + this->boundingBox.size.x &&
           relative.x <= this->boundingBox.start.y + this->boundingBox.size.y;
}

#endif //BUILDIT_POSITIONABLE_H
