//
// Created by felix on 9/6/24.
//

#include "movable.h"

void Movable::move(glm::vec2 newPos) {
    this->onMoveBefore(newPos);
    Subject<MoveEvent>::notify(MoveEvent{this, newPos, true});
    this->pos = newPos;
    this->onMoveAfter(newPos);
    Subject<MoveEvent>::notify(MoveEvent{this, newPos, false});
}

Movable::Movable(glm::vec2 pos, glm::vec2 size) : Positionable(pos, size) {

}
