//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include "wire.h"
#include "graphics/util.h"
#include "simulation/node.h"

Network *Networkable::getNetwork() {
    return this->network;
}

void Networkable::setNetwork(Network *newNetwork) {
    if (newNetwork == this->network) return;
    this->notify({this, newNetwork, true});
    this->network = newNetwork;
    this->notify({this, newNetwork});
}

Networkable::Networkable(Network *network) : network(network) {

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

Wire::Wire(Joint* start, Joint* end, Network* network)
    : start(start), end(end), Networkable(network) {}


Joint::Joint(glm::vec2 pos) : Component(pos, glm::vec2(1, 1)),
        Movable(pos, glm::vec2(1, 1)), Positionable(pos, glm::vec2(1, 1)) {}

Joint::Joint(glm::vec2 pos, Network* network) : Networkable(network), Component(pos, glm::vec2(1, 1)),
        Movable(pos, glm::vec2(1, 1)), Positionable(pos, glm::vec2(1, 1)) {}


Wire* Joint::getWire(Joint* other) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(),
                                   [&other](Wire* wire) {
                                       return wire->start == other || wire->end == other;
                                   });
    if (iter != this->wires.end()) return *iter;
    return nullptr;
}

Joint::~Joint() {
    std::cout << "Deconstructing vertex\n";
}

void Joint::visit(Visitor *visitor) {
    visitor->doFor(this);
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

Network::Network() : hsvColor(Util::random(), 0.8f, 0.65f) {

}

Network::Network(glm::vec3 hsvColor) : hsvColor(hsvColor) {

}

Color Network::getColor() const {
	if (this->parentPin.first != nullptr) {
		SimNodeData simNodeData = this->parentPin.second.getOutputSimData();
		if (simNodeData.node->getOutput(simNodeData.index)) {
		    return Util::hsv2rgb(this->hsvColor - glm::vec3(0, 0.8, 0));
		}
	}
	return Util::hsv2rgb(this->hsvColor);
}

void Network::update() {
    this->notify({this});
}
