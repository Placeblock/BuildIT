//
// Created by felix on 9/1/24.
//

#include "movable.h"

Movable::Movable(glm::vec2 pos) : Positionable(pos) {

}

void Movable::move(glm::vec2 newPos) {
    this->pos = newPos;
}