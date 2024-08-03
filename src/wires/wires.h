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
#include "node.h"
#include "glm/gtx/hash.hpp"

class Wire;
class Network;

class Vertex : public Node {
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
    Wire(std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end);
    std::shared_ptr<Vertex> start;
    std::shared_ptr<Vertex> end;
    std::shared_ptr<Network> network;
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
    void deleteWire(std::shared_ptr<Wire> wire); // vertices are only deleted if they have no more wires
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
    [[nodiscard]] std::shared_ptr<Vertex> getVertex(glm::vec2 cell) const;
    std::shared_ptr<Wire> getWire(glm::vec2 wire);
    std::shared_ptr<Network> getNetwork(std::shared_ptr<Vertex> vertex);
    void deleteVertex(std::shared_ptr<Vertex> vertex);
    void deleteWire(std::shared_ptr<Wire> wire);
    void addVertex(std::shared_ptr<Vertex> vertex);
    void addWire(std::shared_ptr<Wire> wire);

    void fillVertices(std::vector<float>* vertices, std::vector<float> *colors) const;
    void fillWires(std::vector<float>* vertices, std::vector<float> *colors) const;
};


#endif //BUILDIT_WIRES_H
