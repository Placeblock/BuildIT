//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRE_H
#define BUILDIT_WIRE_H

#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include "glm/gtx/hash.hpp"
#include "graphics/types.h"
#include "graphics/circuitBoard/components/nodes/node.h"

class Wire;
class Network;

class Joint {
public:
    glm::vec2 cell;
    glm::vec3 color;
    std::set<Wire*> wires;
    Network* network = nullptr;
    Pin pin;
    Joint(glm::vec2 cell, glm::vec3 color);
    Joint(glm::vec2 cell, glm::vec3 color, Network* network);
    [[nodiscard]] Wire* getWire(Joint* other) const;
    ~Joint() {
        std::cout << "Deconstructing vertex\n";
    }
};

class Wire {
public:
    Wire(Joint* start, Joint* end, glm::vec3 color);
    Wire(Joint* start, Joint* end, Network* network, glm::vec3 color);
    Joint* start = nullptr;
    Joint* end = nullptr;
    Network* network = nullptr;
    glm::vec3 color;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;
    ~Wire() {
        std::cout << "Deconstructing wire\n";
    }
};

class Network {
public:
    glm::vec3 color = glm::vec3(rand()%256, rand()%256, rand()%256);
    std::unordered_set<Wire*> wires;
    std::unordered_set<Joint*> joints;

    Pin parentReference{};
    std::unordered_set<Pin> childReferences;

    void deleteWire(Wire* wire, bool disconnect); // jointVertexData are only deleted if they have no more wires
    void deleteJoint(Joint* joint); // We have to pass
    static void connect(Wire* wire);

    static void connect(Sim::Simulation* sim, Pin parent, Pin child);
    static void disconnect(Sim::Simulation* sim, Pin parent, Pin child);

    ~Network() {
        std::cout << "Deconstructing network " << this << "\n";
    }
};


#endif //BUILDIT_WIRE_H
