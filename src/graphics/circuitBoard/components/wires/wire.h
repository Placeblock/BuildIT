//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRE_H
#define BUILDIT_WIRE_H

#include <vector>
#include <memory>
#include <iostream>

#include "pin.h"

class Wire;
class Network;

class Joint : public Movable{
public:
    std::set<Wire*> wires;
    Network* network = nullptr;
    Pin pin{};
    explicit Joint(glm::vec2 pos);
    Joint(glm::vec2 pos, Network* network);
    [[nodiscard]] Wire* getWire(Joint* other) const;
    ~Joint() override;
};

class Wire {
public:
    Wire(Joint* start, Joint* end);
    Wire(Joint* start, Joint* end, Network* network);
    Joint* start = nullptr;
    Joint* end = nullptr;
    Network* network = nullptr;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;
    ~Wire() {
        std::cout << "Deconstructing wire\n";
    }
};

class Network {
public:
    glm::vec3 hsvColor;
    Network();
    explicit Network(glm::vec3 hsvColor);

    std::unordered_set<Wire*> wires;
    std::unordered_set<Joint*> joints;

    std::pair<Joint*, Pin> parentPin{};
    std::unordered_map<Joint*, Pin> childPins;

	Color getColor() const;
    void removeWire(Wire* wire, bool disconnect); // jointVertexData are only deleted if they have no more wires
    void removeJoint(Joint* joint); // We have to pass
    static void connect(Wire* wire);

    static void connect(Sim::Simulation* sim, const Pin& parent, const Pin& child);
    static void disconnect(Sim::Simulation* sim, const Pin& parent, const Pin& child);

    ~Network() {
        std::cout << "Deconstructing network " << this << "\n";
    }
};


#endif //BUILDIT_WIRE_H
