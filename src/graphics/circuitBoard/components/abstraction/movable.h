//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"

#include "graphics/circuitBoard/observer.h"
#include "component.h"
#include "graphics/circuitBoard/events/moveEvent.h"

class Movable;

class Movable : public Subject<MoveEvent>, virtual public Component {
public:
    virtual void move(glm::vec2 delta);
    virtual void onMove(glm::vec2 delta) {};
};


#endif //BUILDIT_MOVABLE_H
