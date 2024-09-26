//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include <utility>
#include "wire.h"
#include "graphics/util.h"
#include "simulation/node.h"
#include "graphics/circuitBoard/components/nodes/node.h"

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

void Wire::visit(Visitor *visitor) {
    visitor->doFor(this);
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

void Joint::visit(Visitor *visitor) {
    visitor->doFor(this);
}

glm::vec2 Joint::getCenter() const {
    return this->pos;
}

Color Joint::getColor() const {
    return this->isSelected() ? Color{0, 255, 0, 255} : this->getNetwork()->getRenderedColor();
}

void Network::connect(Sim::Simulation* sim, const Pin& parent, const Pin& child) {
    const auto parentRef = Sim::Reference(parent.getOutputSimNode(), child.getInputSimNode(), parent.index);
    const auto childRef = Sim::Reference(child.getInputSimNode(), parent.getOutputSimNode(), child.index);
    sim->connect(parentRef, childRef, true);
}

void Network::disconnect(Sim::Simulation* sim, const Pin& parent, const Pin& child) {
    const auto parentRef = Sim::Reference(parent.getOutputSimNode(), child.getInputSimNode(), parent.index);
    const auto childRef = Sim::Reference(child.getInputSimNode(), parent.getOutputSimNode(), child.index);
    sim->disconnect(parentRef, childRef);
}

Network::Network() : hsvColor(Util::random(), 0.8f, 0.65f), renderedColor(Util::hsv2rgb(this->hsvColor))  {

}

Network::Network(const glm::vec3 hsvColor) : hsvColor(hsvColor), renderedColor(Util::hsv2rgb(hsvColor)) {

}

Color Network::getRenderedColor() const {
	return this->renderedColor;
}

void Network::update() {
    this->notify({this});
}
