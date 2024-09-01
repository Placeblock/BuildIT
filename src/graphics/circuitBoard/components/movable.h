//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"

template<typename S>
class Movable; // We have to forward declare the class as positionable adds Movable as a friend

#include "positionable.h"
#include "graphics/circuitBoard/observer.h"

struct MoveEvent {
    glm::vec2 newPos;
};

template<typename S>
class Movable : public Positionable<S>, public Subject<MoveEvent, S> {
public:
    Movable() = default;
    explicit Movable(glm::vec2 pos);
    virtual void move(glm::vec2 newPos);
    ~Movable() override = default;
};

template<typename S>
Movable<S>::Movable(glm::vec2 pos) : Positionable<S>(pos) {

}

template<typename S>
void Movable<S>::move(glm::vec2 newPos) {
    this->pos = newPos;
    Subject<MoveEvent, S>::notify(this->getSubject(), MoveEvent{newPos});
}

#endif //BUILDIT_MOVABLE_H
