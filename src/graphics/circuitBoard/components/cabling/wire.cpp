//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include <utility>
#include "wire.h"

#include "graphics/constants.h"
#include "graphics/util.h"
#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/circuitBoard/components/cabling/joint.h"

const std::string KEY = "wire";

Joint* Wire::getOther(const Joint* cell) const {
    if (cell == this->start) return this->end;
    return this->start;
}

void Wire::connect() {
	this->start->wires.insert(this);
    this->end->wires.insert(this);
}

void Wire::disconnect() {
	this->start->wires.erase(this);
    this->end->wires.erase(this);
}

Wire::Wire(Joint* start, Joint* end)
    : Movable(Constants::NAMESPACE, KEY), Selectable(Constants::NAMESPACE, KEY),
      LineInteractable(Constants::NAMESPACE, KEY), start(start), end(end) {
}

Wire::Wire(Joint* start, Joint* end, std::shared_ptr<Network> network)
    : Networkable(std::move(network)), Movable(Constants::NAMESPACE, KEY),
      Selectable(Constants::NAMESPACE, KEY), LineInteractable(Constants::NAMESPACE, KEY), start(start), end(end) {}

Wire::Wire(const Wire &other) : Networkable(std::shared_ptr<Network>{}), Movable(other),
                                Selectable(other), LineInteractable(other) {

}

glm::vec2 Wire::getStartPos() const {
    return this->start->getPos();
}

glm::vec2 Wire::getEndPos() const {
    return this->end->getPos();
}

Color Wire::getColor() const {
    return this->isSelected() ? Color{0, 255, 0, 255} : this->getNetwork()->getRenderedColor();
}

void Wire::serialize(SerializationContext &context) {
    const uint32_t startID = context.jointIDs.getID(this->start);
    const uint32_t endID = context.jointIDs.getID(this->end);
    context.serialized.write(reinterpret_cast<const char *>(&startID), sizeof(startID));
    context.serialized.write(reinterpret_cast<const char *>(&endID), sizeof(endID));
}

