//
// Created by felix on 8/2/24.
//

#include <algorithm>
#include "wires.h"

void Network::deleteWire(std::shared_ptr<Wire> wire) {
    wire->start->wires.erase(wire);
    wire->end->wires.erase(wire);
    this->wires.erase(wire);
}

void Network::deleteVertex(std::shared_ptr<Vertex> vertex) {
    this->vertices.erase(vertex);
}

void Network::connect(std::shared_ptr<Wire> wire) {
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
                                   [&wire](const std::pair<std::shared_ptr<Wire>, std::shared_ptr<Network>> cable) {
           const auto left = cable.first->start->cell - wire;
           const auto right = cable.first->end->cell - wire;
           return left.x*right.y - left.y*right.x == 0 &&
                  left.x*right.x + left.y*right.y < 0;
       });
    if (iter != this->wireMap.end()) return iter->first;
    return nullptr;
}

std::shared_ptr<Network> Wires::getNetwork(std::shared_ptr<Vertex> vertex) {
    if (const auto result = this->vertexMap.find(vertex); result != this->vertexMap.end()) {
        return result->second;
    }
    return nullptr;
}

void Wires::deleteVertex(std::shared_ptr<Vertex> vertex) {
    this->vertexMap.erase(vertex);
    this->cellMap.erase(vertex->cell);
    vertex->network->deleteVertex(vertex);
}

void Wires::deleteWire(std::shared_ptr<Wire> wire) {
    this->wireMap.erase(wire);
    wire->network->deleteWire(wire);
}

void Wires::fillVertices(std::vector<float> *vertices, std::vector<float> *colors) const {
    for (const auto &vertex: this->vertexMap) {
        vertices->push_back(vertex.first->cell.x * 32);
        vertices->push_back(vertex.first->cell.y * 32);
        colors->push_back(vertex.first->color.x);
        colors->push_back(vertex.first->color.y);
        colors->push_back(vertex.first->color.z);
    }
}

void Wires::fillWires(std::vector<float> *vertices, std::vector<float> *colors) const {
    for (const auto &cable: this->wireMap) {
        vertices->push_back(cable.first->start->cell.x * 32);
        vertices->push_back(cable.first->start->cell.y * 32);
        vertices->push_back(cable.first->end->cell.x * 32);
        vertices->push_back(cable.first->end->cell.y * 32);
        colors->push_back(cable.first->network->color.x); // We need to do it twice (for each vertex)
        colors->push_back(cable.first->network->color.y);
        colors->push_back(cable.first->network->color.z);
        colors->push_back(cable.first->network->color.x);
        colors->push_back(cable.first->network->color.y);
        colors->push_back(cable.first->network->color.z);
    }
}

void Wires::addVertex(std::shared_ptr<Vertex> vertex) {
    this->vertexMap[vertex] = vertex->network;
    this->cellMap[vertex->cell] = vertex;
    vertex->network->vertices.insert(vertex);
}

void Wires::addWire(std::shared_ptr<Wire> wire) {
    this->wireMap[wire] = wire->network;
    wire->network->wires.insert(wire);
}

std::shared_ptr<Vertex> Wire::getOther(std::shared_ptr<Vertex> cell) {
    if (cell == this->start) return this->end;
    return this->start;
}

Wire::Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end) {
    this->start = start;
    this->end = end;
}

Wire::Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end, std::shared_ptr<Network> network) {
    this->start = start;
    this->end = end;
    this->network = network;
}

Vertex::Vertex(glm::vec2 cell, glm::vec3 color) {
    this->cell = cell;
    this->color = color;
}
