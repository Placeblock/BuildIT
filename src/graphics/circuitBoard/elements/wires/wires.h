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
    glm::vec2 pos;
    glm::vec3 color;
    std::set<Wire*> wires;
    Network* network = nullptr;
    Vertex(glm::vec2 cell, glm::vec3 color);
    Vertex(glm::vec2 cell, glm::vec3 color, Network* network);
    [[nodiscard]] Wire* getWire(Vertex* other) const;
    ~Vertex() {
        std::cout << "Deconstructing vertex\n";
    }
};

class Wire {
public:
    Wire(Vertex* start, Vertex* end, glm::vec3 color);
    Wire(Vertex* start, Vertex* end, Network* network, glm::vec3 color);
    Vertex* start = nullptr;
    Vertex* end = nullptr;
    Network* network = nullptr;
    glm::vec3 color;
    [[nodiscard]] Vertex* getOther(const Vertex* cell) const;
    ~Wire() {
        std::cout << "Deconstructing wire\n";
    }
};

class Network {
public:
    glm::vec3 color = glm::vec3(rand()%256, rand()%256, rand()%256);
    std::unordered_set<Wire*> wires;
    std::unordered_set<Vertex*> vertices;
    Sim::Reference inputReference;
    std::vector<Sim::Reference> outputReferences;
    void deleteWire(Wire* wire, bool disconnect); // vertexData are only deleted if they have no more wires
    void deleteVertex(Vertex* vertex);
    static void connect(Wire* wire);
    ~Network() {
        std::cout << "Deconstructing network " << this << "\n";
    }
};

class Wires {
public:
    std::unordered_set<std::shared_ptr<Network>> networks;
    std::set<std::shared_ptr<Vertex>> vertices;
    std::set<std::shared_ptr<Wire>> wires;
    std::unordered_map<intVec2, Vertex*> cellMap;
    std::unordered_map<Vertex*, Network*> vertexMap;
    std::unordered_map<Wire*, Network*> wireMap;
    [[nodiscard]] Vertex* getVertex(intVec2 cell) const;
    Wire* getWire(glm::vec2 pair);
    Network* getNetwork(Vertex* vertex);
    void deleteVertex(Vertex* vertex);
    void deleteWire(Wire* wire);
    void addVertex(const std::shared_ptr<Vertex>& vertex);
    void addWire(const std::shared_ptr<Wire>& wire);
    [[nodiscard]] long getVertexIndex(const Vertex* vertex) const;
    [[nodiscard]] long getWireIndex(const Wire* wire) const;

    [[nodiscard]] std::shared_ptr<Vertex> getOwningRef(const Vertex* vertex) const;
    [[nodiscard]] std::shared_ptr<Wire> getOwningRef(const Wire* wire) const ;
    [[nodiscard]] std::shared_ptr<Network> getOwningRef(const Network* network) const ;

    std::set<const Vertex*> getNonOwningVertices() const;
    std::set<const Wire*> getNonOwningWires() const;
};


#endif //BUILDIT_WIRES_H
