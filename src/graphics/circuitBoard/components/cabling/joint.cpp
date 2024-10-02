//
// Created by felix on 9/29/24.
//

#include <ranges>
#include "joint.h"
#include "wire.h"
#include "graphics/constants.h"

const std::string KEY = "joint";

Joint::Joint(const glm::vec2 pos) : Component(Constants::NAMESPACE, KEY), Movable(Constants::NAMESPACE, KEY),
    Selectable(Constants::NAMESPACE, KEY), CircleInteractable(Constants::NAMESPACE, KEY, 10), pos(pos) {}

Joint::Joint(const glm::vec2 pos, std::shared_ptr<Network> network)
        : Component(Constants::NAMESPACE, KEY), Networkable(std::move(network)), Movable(Constants::NAMESPACE, KEY),
          Selectable(Constants::NAMESPACE, KEY),
          CircleInteractable(Constants::NAMESPACE, KEY, 10), pos(pos) {}

Joint::Joint(const Joint &other) : Component(other), Networkable(std::shared_ptr<Network>{}),
                                   Movable(Constants::NAMESPACE, KEY), Selectable(Constants::NAMESPACE, KEY),
                                   CircleInteractable(Constants::NAMESPACE, KEY, 10), pos(other.pos) {

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

void Joint::serialize(SerializationContext &context) {
    const uint32_t id = context.jointIDs.getID(this);
    context.serialized.write(reinterpret_cast<const char *>(&id), sizeof(id));
    const auto cell = intVec2(this->getPos() / 32.0f);
    context.serialized.write(reinterpret_cast<const char *>(&cell.x), sizeof(cell.x));
    context.serialized.write(reinterpret_cast<const char *>(&cell.y), sizeof(cell.y));
    const uint32_t networkID = context.networkIDs.getID(this->getNetwork().get());
    context.serialized.write(reinterpret_cast<const char *>(&networkID), sizeof(networkID));
}