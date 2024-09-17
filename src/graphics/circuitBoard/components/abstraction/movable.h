//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"

#include "positionable.h"
#include "graphics/circuitBoard/observer.h"

class Movable;

struct MoveEvent {
    Movable *movable;
    glm::vec2 newPos;
    bool before = false;
};

class Movable : public Positionable, public Subject<MoveEvent> {
public:
    Movable(glm::vec2 pos, glm::vec2 size);
    virtual void move(glm::vec2 newPos);
    virtual void onMoveBefore(glm::vec2 newPos) {};
    virtual void onMoveAfter(glm::vec2 newPos) {};

    ~Movable() override = default;
};


#endif //BUILDIT_MOVABLE_H
