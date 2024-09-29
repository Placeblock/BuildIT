//
// Created by felix on 9/29/24.
//

#include <ranges>
#include "joint.h"
#include "wire.h"

Joint::Joint(const glm::vec2 pos) : CircleInteractable(10), pos(pos) {}

Joint::Joint(const glm::vec2 pos, std::shared_ptr<Network> network)
        : Networkable(std::move(network)), CircleInteractable(10), pos(pos) {}

Joint::Joint(Joint &other) : Networkable(std::shared_ptr<Network>{}), CircleInteractable(10), pos(other.pos) {

}


Wire* Joint::getWire(Joint* other) const {
    const auto iter = std::ranges::find_if(this->wires,
                                           [&other](const Wire* wire) {
                                               return wire->start == other || wire->end == other;
                                           });
    if (iter != this->wires.end()) return *iter;
    return nullptr;
}

void Joint::onMove(const glm::vec2 delta) {
    this->pos += delta;
}

glm::vec2 Joint::getPos() const {
    return this->pos;
}

glm::vec2 Joint::getCenter() const {
    return this->pos;
}

Color Joint::getColor() const {
    return this->isSelected() ? Color{0, 255, 0, 255} : this->getNetwork()->getRenderedColor();
}