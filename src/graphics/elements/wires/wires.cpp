//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include <utility>
#include "wires.h"

void Network::deleteWire(const std::shared_ptr<Wire>& wire) {
    wire->start->wires.erase(wire);
    wire->end->wires.erase(wire);
    this->wires.erase(wire);
}

void Network::deleteVertex(const std::shared_ptr<Vertex>& vertex) {
    this->vertices.erase(vertex);
}

void Network::connect(const std::shared_ptr<Wire>& wire) {
    wire->start->wires.insert(wire);
    wire->end->wires.insert(wire);
}

std::shared_ptr<Vertex> Wires::getVertex(glm::vec2 cell) const {
    if (const auto result = this->cellMap.find(cell); result != this->cellMap.end()) {
        return result->second;
    }
    return nullptr;
}

std::shared_ptr<Wire> Wires::getWire(glm::vec2 wire) {
    const auto iter = std::find_if(this->wireMap.begin(), this->wireMap.end(),
                                   [&wire](const std::pair<std::shared_ptr<Wire>, std::shared_ptr<Network>>& cable) {
        const auto left = cable.first->start->cell - wire;
        const auto right = cable.first->end->cell - wire;
        return left.x*right.y - left.y*right.x == 0 &&
            left.x*right.x + left.y*right.y < 0;
       });
    if (iter != this->wireMap.end()) return iter->first;
    return nullptr;
}

std::shared_ptr<Network> Wires::getNetwork(const std::shared_ptr<Vertex>& vertex) {
    if (const auto result = this->vertexMap.find(vertex); result != this->vertexMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Wires::deleteVertex(const std::shared_ptr<Vertex>& vertex) {
    this->vertexMap.erase(vertex);
    this->vertices.erase(vertex);
    this->cellMap.erase(vertex->cell);
    vertex->network->deleteVertex(vertex);
}

void Wires::deleteWire(const std::shared_ptr<Wire>& wire) {
    this->wireMap.erase(wire);
    this->wires.erase(wire);
    wire->network->deleteWire(wire);
}

void Wires::addVertex(const std::shared_ptr<Vertex>& vertex) {
    this->vertexMap[vertex] = vertex->network;
    this->cellMap[vertex->cell] = vertex;
    this->vertices.insert(vertex);
    vertex->network->vertices.insert(vertex);
}

void Wires::addWire(const std::shared_ptr<Wire>& wire) {
    this->wireMap[wire] = wire->network;
    this->wires.insert(wire);
    wire->network->wires.insert(wire);
}

long Wires::getVertexIndex(const std::shared_ptr<Vertex>& vertex) {
    return std::distance(this->vertices.begin(), this->vertices.find(vertex));
}

long Wires::getWireIndex(const std::shared_ptr<Wire>& wire) {
    return std::distance(this->wires.begin(), this->wires.find(wire));
}

std::shared_ptr<Vertex> Wire::getOther(const std::shared_ptr<Vertex>& cell) const {
    if (cell == this->start) return this->end;
    return this->start;
}

Wire::Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end, glm::vec3 color)
    : start(std::move(start)), end(std::move(end)), color(color){}

Wire::Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end, std::shared_ptr<Network> network, glm::vec3 color)
    : start(std::move(start)), end(std::move(end)), color(color), network(std::move(network)) {}

Vertex::Vertex(glm::vec2 cell, glm::vec3 color) : cell(cell), color(color) {}

Vertex::Vertex(glm::vec2 cell, glm::vec3 color, std::shared_ptr<Network> network) : cell(cell), color(color), network(std::move(network)) {}
