//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include "wire.h"

void Network::deleteWire(Wire* wire, bool disconnect) {
    if (disconnect) {
        wire->start->wires.erase(wire);
        wire->end->wires.erase(wire);
    }
    this->wires.erase(wire);
}

void Network::deleteJoint(Joint* joint) {
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

Wire::Wire(Joint* start, Joint* end, glm::vec3 color)
    : start(start), end(end), color(color){}

Wire::Wire(Joint* start, Joint* end, Network* network, glm::vec3 color)
    : start(start), end(end), color(color), network(network) {}

Joint::Joint(glm::vec2 cell, glm::vec3 color) : cell(cell), color(color) {}

Joint::Joint(glm::vec2 cell, glm::vec3 color, Network* network) : cell(cell), color(color), network(network) {}

Wire* Joint::getWire(Joint* other) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(),
                                   [&other](Wire* wire) {
                                       return wire->start == other || wire->end == other;
                                   });
    if (iter != this->wires.end()) return *iter;
    return nullptr;
}

void Network::connect(Sim::Simulation* sim, Pin parent, Pin child) {
    const auto parentRef = Sim::Reference(parent.node->simNode.get(), child.node->simNode.get(), parent.index);
    const auto childRef = Sim::Reference(child.node->simNode.get(), parent.node->simNode.get(), child.index);
    sim->connect(parentRef, childRef, true);
}

void Network::disconnect(Sim::Simulation* sim, Pin parent, Pin child) {
    const auto parentRef = Sim::Reference(parent.node->simNode.get(), child.node->simNode.get(), parent.index);
    const auto childRef = Sim::Reference(child.node->simNode.get(), parent.node->simNode.get(), child.index);
    sim->disconnect(parentRef, childRef);
}
