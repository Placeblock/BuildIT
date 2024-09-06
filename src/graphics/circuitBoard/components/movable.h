//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"

class Movable; // We have to forward declare the class as positionable adds Movable as a friend

#include "positionable.h"
#include "graphics/circuitBoard/observer.h"

struct MoveEvent {
    glm::vec2 newPos;
};

class Movable : public Positionable, public Subject<MoveEvent> {
public:
    Movable() = default;
    explicit Movable(glm::vec2 pos);
    virtual void move(glm::vec2 newPos);
    ~Movable() override = default;
};



#endif //BUILDIT_MOVABLE_H
