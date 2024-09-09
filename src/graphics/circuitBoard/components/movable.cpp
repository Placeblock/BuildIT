//
// Created by felix on 9/6/24.
//

#include "movable.h"


Movable::Movable(glm::vec2 pos) : Positionable(pos) {

}

void Movable::move(glm::vec2 newPos) {
    Subject<MoveEvent>::notify(MoveEvent{newPos, true});
    this->pos = newPos;
    Subject<MoveEvent>::notify(MoveEvent{newPos, false});
}