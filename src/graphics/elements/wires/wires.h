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
#include "simulation/node.h"
#include "graphics/types.h"

class Wire;
class Network;

class Vertex {
public:
    glm::vec2 cell;
    glm::vec3 color;
    std::set<std::shared_ptr<Wire>> wires;
    std::shared_ptr<Network> network;
    Vertex(glm::vec2 cell, glm::vec3 color);
    Vertex(glm::vec2 cell, glm::vec3 color, std::shared_ptr<Network> network);
    [[nodiscard]] std::shared_ptr<Wire> getWire(std::shared_ptr<Vertex> other) const;
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
    [[nodiscard]] std::shared_ptr<Vertex> getOther(const std::shared_ptr<Vertex>& cell) const;
    ~Wire() {
        std::cout << "Deconstructing wire\n";
    }
};

class Network {
public:
    glm::vec3 color;
    std::unordered_set<std::shared_ptr<Wire>> wires;
    std::unordered_set<std::shared_ptr<Vertex>> vertices;
    Sim::Reference inputReference;
    std::vector<Sim::Reference> outputReferences;
    void deleteWire(const std::shared_ptr<Wire>& wire, bool disconnect); // vertexData are only deleted if they have no more wires
    void deleteVertex(const std::shared_ptr<Vertex>& vertex);
    static void connect(const std::shared_ptr<Wire>& wire);
    ~Network() {
        std::cout << "Deconstructing network " << this << "\n";
    }
};

class Wires {
public:
    std::unordered_set<std::shared_ptr<Network>> networks;
    std::unordered_map<intVec2, std::shared_ptr<Vertex>> cellMap;
    std::unordered_map<std::shared_ptr<Vertex>, std::shared_ptr<Network>> vertexMap;
    std::unordered_map<std::shared_ptr<Wire>, std::shared_ptr<Network>> wireMap;
    std::set<std::shared_ptr<Vertex>> vertices;
    std::set<std::shared_ptr<Wire>> wires;
    [[nodiscard]] std::shared_ptr<Vertex> getVertex(intVec2 cell) const;
    std::shared_ptr<Wire> getWire(glm::vec2 pair);
    std::shared_ptr<Network> getNetwork(const std::shared_ptr<Vertex>& vertex);
    void deleteVertex(const std::shared_ptr<Vertex>& vertex);
    void deleteWire(const std::shared_ptr<Wire>& wire);
    void addVertex(const std::shared_ptr<Vertex>& vertex);
    void addWire(const std::shared_ptr<Wire>& wire);
    long getVertexIndex(const std::shared_ptr<Vertex>& vertex);
    long getWireIndex(const std::shared_ptr<Wire>& wire);
};


#endif //BUILDIT_WIRES_H
