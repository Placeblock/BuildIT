//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"

#include "graphics/circuitBoard/observer.h"
#include "component.h"

class Movable;

struct MoveEvent {
    Movable *movable;
    glm::vec2 delta;
    bool before = false;
};

class Movable : public Subject<MoveEvent>, virtual public Component {
public:
    virtual void move(glm::vec2 delta);
    virtual void onMove(glm::vec2 delta) = 0;
};


#endif //BUILDIT_MOVABLE_H
