//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include <utility>
#include "wires.h"

void Network::deleteWire(Wire* wire, bool disconnect) {
    if (disconnect) {
        wire->start->wires.erase(wire);
        wire->end->wires.erase(wire);
    }
    this->wires.erase(wire);
}

void Network::deleteVertex(Vertex* vertex) {
    this->vertices.erase(vertex);
    if (this->parentReference.first == vertex) {
        this->parentReference = {};
    }
    this->childReferences.erase(vertex);
}

void Network::connect(Wire* wire) {
    wire->start->wires.insert(wire);
    wire->end->wires.insert(wire);
}

void Network::connect(Sim::Simulation* sim, Pin parent, Pin child) {
    const auto parentRef = Sim::Reference(parent.node->simNode.get(), child.node->simNode.get(), parent.index);
    const auto childRef = Sim::Reference(child.node->simNode.get(), parent.node->simNode.get(), child.index);
    sim->connect(parentRef, childRef);
}

Vertex* Wires::getVertex(intVec2 cell) const {
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

Network* Wires::getNetwork(Vertex* vertex) {
    if (const auto result = this->vertexMap.find(vertex); result != this->vertexMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Wires::deleteVertex(Vertex* vertex) {
    this->vertexMap.erase(vertex);
    const auto iter = std::find_if(this->vertices.begin(), this->vertices.end(), [&vertex](const std::shared_ptr<Vertex>& v){
        return v.get() == vertex;
    });
    this->cellMap.erase(vertex->cell);
    vertex->network->deleteVertex(vertex);
    this->vertices.erase(iter);
}

void Wires::deleteWire(Wire* wire) {
    this->wireMap.erase(wire);
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    wire->network->deleteWire(wire, true);
    this->wires.erase(iter);
}

void Wires::addVertex(const std::shared_ptr<Vertex>& vertex) {
    this->vertexMap[vertex.get()] = vertex->network;
    this->cellMap[vertex->cell] = vertex.get();
    this->vertices.insert(vertex);
    vertex->network->vertices.insert(vertex.get());
}

void Wires::addWire(const std::shared_ptr<Wire>& wire) {
    this->wireMap[wire.get()] = wire->network;
    this->wires.insert(wire);
    wire->network->wires.insert(wire.get());
}

long Wires::getVertexIndex(const Vertex* vertex) const {
    const auto iter = std::find_if(this->vertices.begin(), this->vertices.end(), [&vertex](const std::shared_ptr<Vertex>& v){
        return v.get() == vertex;
    });
    return std::distance(this->vertices.begin(), iter);
}

long Wires::getWireIndex(const Wire* wire) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(), [&wire](const std::shared_ptr<Wire>& w){
        return w.get() == wire;
    });
    return std::distance(this->wires.begin(), iter);
}

std::shared_ptr<Vertex> Wires::getOwningRef(const Vertex *vertex) const {
    const auto iter = std::find_if(this->vertices.begin(), this->vertices.end(), [&vertex](const std::shared_ptr<Vertex>& v){
        return v.get() == vertex;
    });
    if (iter == this->vertices.end()) return nullptr;
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

std::set<const Vertex *> Wires::getNonOwningVertices() const {
    std::set<const Vertex*> nOVertices;
    std::transform(this->vertices.begin(), this->vertices.end(), std::inserter(nOVertices, nOVertices.end()), [](const auto& v) {
        return v.get();
    });
    return nOVertices;
}

std::set<const Wire *> Wires::getNonOwningWires() const {
    std::set<const Wire*> nOWires;
    std::transform(this->wires.begin(), this->wires.end(), std::inserter(nOWires, nOWires.end()), [](const auto& v) {
        return v.get();
    });
    return nOWires;
}

Vertex* Wire::getOther(const Vertex* cell) const {
    if (cell == this->start) return this->end;
    return this->start;
}

Wire::Wire(Vertex* start, Vertex* end, glm::vec3 color)
    : start(start), end(end), color(color){}

Wire::Wire(Vertex* start, Vertex* end, Network* network, glm::vec3 color)
    : start(start), end(end), color(color), network(network) {}

Vertex::Vertex(glm::vec2 cell, glm::vec3 color) : cell(cell), color(color) {}

Vertex::Vertex(glm::vec2 cell, glm::vec3 color, Network* network) : cell(cell), color(color), network(network) {}

Wire* Vertex::getWire(Vertex* other) const {
    const auto iter = std::find_if(this->wires.begin(), this->wires.end(),
                                   [&other](Wire* wire) {
                                       return wire->start == other || wire->end == other;
                                   });
    if (iter != this->wires.end()) return *iter;
    return nullptr;
}
