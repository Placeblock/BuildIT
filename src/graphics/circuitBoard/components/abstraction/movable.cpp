//
// Created by felix on 9/6/24.
//

#include "movable.h"

void Movable::move(glm::vec2 delta) {
    Subject<MoveEvent>::notify(MoveEvent{this, delta, true});
    this->onMove(delta);
    Subject<MoveEvent>::notify(MoveEvent{this, delta, false});
}
