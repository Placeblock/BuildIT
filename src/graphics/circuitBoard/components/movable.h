//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_MOVABLE_H
#define BUILDIT_MOVABLE_H

#include "glm/vec2.hpp"

// The generic type is only to allow inheritance of multiple MoveEvent observables
template <typename T>
class Movable; // We have to forward declare the class as positionable adds Movable as a friend

#include "positionable.h"
#include "graphics/circuitBoard/observer.h"

template <typename T>
struct MoveEvent {
    glm::vec2 newPos;
    bool before = false;
};

template <typename T>
class Movable : public Positionable<T>, public Subject<MoveEvent<T>> {
public:
    Movable() = default;
    explicit Movable(glm::vec2 pos);
    virtual void move(glm::vec2 newPos);
    ~Movable() override = default;
};

template <typename T>
Movable<T>::Movable(glm::vec2 pos) : Positionable<T>(pos) {

}

template <typename T>
void Movable<T>::move(glm::vec2 newPos) {
    Subject<MoveEvent<T>>::notify(MoveEvent<T>{newPos, true});
    this->pos = newPos;
    Subject<MoveEvent<T>>::notify(MoveEvent<T>{newPos, false});
}


#endif //BUILDIT_MOVABLE_H
