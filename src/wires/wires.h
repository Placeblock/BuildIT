//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRES_H
#define BUILDIT_WIRES_H

#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <iostream>
#include "glm/gtx/hash.hpp"
#include "nodes/gate.h"

class Wire;
class Network;

class Vertex {
public:
    glm::vec2 cell;
    glm::vec3 color;
    std::set<std::shared_ptr<Wire>> wires;
    std::shared_ptr<Network> network;
    Vertex(glm::vec2 cell, glm::vec3 color);
    ~Vertex() {
        std::cout << "Deconstructing vertex\n";
    }
};

class Wire {
public:
    Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end, glm::vec3 color);
    Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end, std::shared_ptr<Network> network, glm::vec3 color);
    std::shared_ptr<Vertex> start;
    std::shared_ptr<Vertex> end;
    std::shared_ptr<Network> network;
    glm::vec3 color;
    std::shared_ptr<Vertex> getOther(std::shared_ptr<Vertex> cell);
    ~Wire() {
        std::cout << "Deconstructing wire\n";
    }
};

class Network {
public:
    glm::vec3 color;
    std::unordered_set<std::shared_ptr<Wire>> wires;
    std::unordered_set<std::shared_ptr<Vertex>> vertices;
    std::shared_ptr<Gate> inputGate;
    std::unordered_set<std::shared_ptr<Gate>> outputGates;
    void deleteWire(std::shared_ptr<Wire> wire); // vertexData are only deleted if they have no more wires
    void deleteVertex(std::shared_ptr<Vertex> vertex);
    void connect(std::shared_ptr<Wire> wire);
    ~Network() {
        std::cout << "Deconstructing network\n";
    }
};

class Wires {
public:
    std::unordered_set<std::shared_ptr<Network>> networks;
    std::unordered_map<glm::vec2, std::shared_ptr<Vertex>> cellMap;
    std::unordered_map<std::shared_ptr<Vertex>, std::shared_ptr<Network>> vertexMap;
    std::unordered_map<std::shared_ptr<Wire>, std::shared_ptr<Network>> wireMap;
    std::set<std::shared_ptr<Vertex>> vertices;
    std::set<std::shared_ptr<Wire>> wires;
    [[nodiscard]] std::shared_ptr<Vertex> getVertex(glm::vec2 cell) const;
    std::shared_ptr<Wire> getWire(glm::vec2 wire);
    std::shared_ptr<Network> getNetwork(std::shared_ptr<Vertex> vertex);
    void deleteVertex(std::shared_ptr<Vertex> vertex);
    void deleteWire(std::shared_ptr<Wire> wire);
    void addVertex(std::shared_ptr<Vertex> vertex);
    void addWire(std::shared_ptr<Wire> wire);
    long getVertexIndex(std::shared_ptr<Vertex> vertex);
    long getWireIndex(std::shared_ptr<Wire> wire);
};


#endif //BUILDIT_WIRES_H
