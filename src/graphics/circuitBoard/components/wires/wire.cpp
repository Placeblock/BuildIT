//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include "wire.h"
#include "graphics/util.h"
#include "simulation/node.h"

void Network::removeWire(Wire* wire, bool disconnect) {
    if (disconnect) {
        wire->start->wires.erase(wire);
        wire->end->wires.erase(wire);
    }
    this->wires.erase(wire);
}

void Network::removeJoint(Joint* joint) {
    this->joints.erase(joint);
}

void Network::connect(Wire* wire) {
    wire->start->wires.insert(wire);
    wire->end->wires.insert(wire);
}

Joint* Wire::getOther(const Joint* cell) const {
    if (cell == this->start) return this->end;
    return this->start;
}

Wire::Wire(Joint* start, Joint* end)
    : start(start), end(end) {}

Wire::Wire(Joint* start, Joint* end, Network* network)
    : start(start), end(end), network(network) {}

Joint::Joint(glm::vec2 cell) : cell(cell) {}

Joint::Joint(glm::vec2 cell, Network* network) : cell(cell), network(network) {}

Wire* Joint::getWire(Joint* other) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(),
                                   [&other](Wire* wire) {
                                       return wire->start == other || wire->end == other;
                                   });
    if (iter != this->wires.end()) return *iter;
    return nullptr;
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
