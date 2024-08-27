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

class Joint {
public:
    glm::vec2 cell;
    glm::vec3 color;
    std::set<Wire*> wires;
    Network* network = nullptr;
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
    void deleteWire(Wire* wire, bool disconnect); // vertexData are only deleted if they have no more wires
    void deleteJoint(Joint* joint); // We have to pass
    static void connect(Wire* wire);
    ~Network() {
        std::cout << "Deconstructing network " << this << "\n";
    }
};

class Wires {
public:
    std::unordered_set<std::shared_ptr<Network>> networks;
    std::set<std::shared_ptr<Joint>> joints;
    std::set<std::shared_ptr<Wire>> wires;
    std::unordered_map<intVec2, Joint*> cellMap;
    std::unordered_map<Joint*, Network*> jointMap;
    std::unordered_map<Wire*, Network*> wireMap;
    [[nodiscard]] Joint* getJoint(intVec2 cell) const;
    Wire* getWire(glm::vec2 pair);
    Network* getNetwork(Joint* joint);
    void deleteJoint(Joint* joint);
    void deleteWire(Wire* wire);
    void addJoint(const std::shared_ptr<Joint>& joint);
    void addWire(const std::shared_ptr<Wire>& wire);
    [[nodiscard]] long getJointIndex(const Joint* joint) const;
    [[nodiscard]] long getWireIndex(const Wire* wire) const;

    [[nodiscard]] std::shared_ptr<Joint> getOwningRef(const Joint* joint) const;
    [[nodiscard]] std::shared_ptr<Wire> getOwningRef(const Wire* wire) const ;
    [[nodiscard]] std::shared_ptr<Network> getOwningRef(const Network* network) const ;

    std::set<const Joint*> getNonOwningJoints() const;
    std::set<const Wire*> getNonOwningWires() const;
};


#endif //BUILDIT_WIRES_H
