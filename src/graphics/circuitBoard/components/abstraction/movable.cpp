//
// Created by felix on 9/6/24.
//

#include "movable.h"

#include <utility>

void Movable::move(const glm::vec2 delta) {
    Subject::notify(MoveEvent{this, delta, true});
    this->onMove(delta);
    Subject::notify(MoveEvent{this, delta, false});
}

Movable::Movable(std::string cnamespace, std::string ckey) : Component(std::move(cnamespace), std::move(ckey)) {

}

Movable::Movable(const Movable &other) : Component(other) {
}