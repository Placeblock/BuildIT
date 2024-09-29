//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include <utility>
#include "wire.h"
#include "graphics/util.h"
#include "simulation/node.h"
#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/circuitBoard/components/cabling/joint.h"

std::shared_ptr<Network> Networkable::getNetwork() const {
    return this->network;
}

void Networkable::setNetwork(std::shared_ptr<Network> newNetwork) {
    if (newNetwork == this->network) return;
    this->notify({this, newNetwork.get(), true});
    this->network = std::move(newNetwork);
    this->notify({this, this->network.get()});
}

Networkable::Networkable(std::shared_ptr<Network> network) : network(std::move(network)) {

}

void Network::removeWire(Wire* wire) {
    this->wires.remove(wire);
}

void Network::removeJoint(Joint* joint) {
    this->joints.remove(joint);
}


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
    : start(start), end(end) {}

Wire::Wire(Joint* start, Joint* end, std::shared_ptr<Network> network)
    : Networkable(std::move(network)), start(start), end(end) {}

Wire::Wire(Wire &other) : Networkable(std::shared_ptr<Network>{}) {

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

}

