//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include "wires.h"

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

Joint* Wires::getJoint(intVec2 cell) const {
    if (const auto result = this->cellMap.find(cell); result != this->cellMap.end()) {
        return result->second;
    }
    return nullptr;
}

Wire* Wires::getWire(glm::vec2 wire) {
    const auto iter = std::find_if(this->wireMap.begin(), this->wireMap.end(),
                                   [&wire](const auto& pair) {
        const glm::vec2 left = pair.first->start->cell - wire;
        const glm::vec2 right = pair.first->end->cell - wire;
        return left.x*right.y - left.y*right.x == 0 &&
            left.x*right.x + left.y*right.y < 0;
       });
    if (iter != this->wireMap.end()) return iter->first;
    return nullptr;
}

Network* Wires::getNetwork(Joint* joint) {
    if (const auto result = this->jointMap.find(joint); result != this->jointMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Wires::removeJoint(Joint* joint) {
    this->jointMap.erase(joint);
    const auto iter = std::find_if(this->joints.begin(), this->joints.end(), [&joint](const std::shared_ptr<Joint>& v){
        return v.get() == joint;
    });
    this->cellMap.erase(joint->cell);
    joint->network->deleteJoint(joint);
    this->joints.erase(iter);
}

void Wires::removeWire(Wire* wire) {
    this->wireMap.erase(wire);
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    wire->network->deleteWire(wire, true);
    this->wires.erase(iter);
}

void Wires::addJoint(const std::shared_ptr<Joint>& joint) {
    this->jointMap[joint.get()] = joint->network;
    this->cellMap[joint->cell] = joint.get();
    this->joints.insert(joint);
    joint->network->joints.insert(joint.get());
}

void Wires::addWire(const std::shared_ptr<Wire>& wire) {
    this->wireMap[wire.get()] = wire->network;
    this->wires.insert(wire);
    wire->network->wires.insert(wire.get());
}

long Wires::getJointIndex(const Joint* joint) const {
    const auto iter = std::find_if(this->joints.begin(), this->joints.end(), [&joint](const std::shared_ptr<Joint>& j){
        return j.get() == joint;
    });
    return std::distance(this->joints.begin(), iter);
}

long Wires::getWireIndex(const Wire* wire) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    return std::distance(this->wires.begin(), iter);
}

std::shared_ptr<Joint> Wires::getOwningRef(const Joint *joint) const {
    const auto iter = std::find_if(this->joints.begin(), this->joints.end(), [&joint](const std::shared_ptr<Joint>& j){
        return j.get() == joint;
    });
    if (iter == this->joints.end()) return nullptr;
    return *iter;
}

std::shared_ptr<Wire> Wires::getOwningRef(const Wire *wire) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    if (iter == this->wires.end()) return nullptr;
    return *iter;
}

std::shared_ptr<Network> Wires::getOwningRef(const Network *network) const {
    const auto iter = std::find_if(this->networks.begin(), this->networks.end(), [&network](const std::shared_ptr<Network>& n){
        return n.get() == network;
    });
    if (iter == this->networks.end()) return nullptr;
    return *iter;
}

std::set<const Joint *> Wires::getNonOwningJoints() const {
    std::set<const Joint*> nOVertices;
    std::transform(this->joints.begin(), this->joints.end(), std::inserter(nOVertices, nOVertices.end()), [](const auto& j) {
        return j.get();
    });
    return nOVertices;
}

std::set<const Wire *> Wires::getNonOwningWires() const {
    std::set<const Wire*> nOWires;
    std::transform(this->wires.begin(), this->wires.end(), std::inserter(nOWires, nOWires.end()), [](const auto& w) {
        return w.get();
    });
    return nOWires;
}

void Wires::moveJoint(Joint *joint, glm::vec2 newCell) {
    if (this->cellMap[joint->cell] == joint) { // When moving multiple this could be false
        this->cellMap.erase(joint->cell);
    }
    joint->cell = newCell;
    this->cellMap[newCell] = joint;
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
