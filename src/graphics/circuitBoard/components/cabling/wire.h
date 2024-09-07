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
class Joint;

struct NetworkAddEvent {
    Network *network;
};

struct NetworkRemoveEvent {
    Network *network;
};

template <typename T>
struct NetworkChangeEvent {
    Network *newNetwork;
    bool before = false;
};

struct NetworkUpdateEvent {};

struct JointAddEvent {
    Joint *joint;
};

struct JointRemoveEvent {
    Joint *joint;
};

struct WireAddEvent {
    Wire *wire;
};

struct WireRemoveEvent {
    Wire *wire;
};

class Joint : public Movable<Joint>, public Subject<NetworkChangeEvent<Joint>> {
private:
    Network* network = nullptr;
public:
    std::set<Wire*> wires;
    Pin pin{};

    explicit Joint(glm::vec2 pos);
    Joint(glm::vec2 pos, Network* network);

    Network* getNetwork();
    void setNetwork(Network *newNetwork);

    [[nodiscard]] Wire* getWire(Joint* other) const;

    ~Joint() override;
};

class Wire : public Subject<NetworkChangeEvent<Wire>> {
private:
    Network* network = nullptr;
public:
    Wire(Joint* start, Joint* end);
    Wire(Joint* start, Joint* end, Network* network);
    Joint* start = nullptr;
    Joint* end = nullptr;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;

    Network* getNetwork();
    void setNetwork(Network *newNetwork);

    ~Wire() override {
        std::cout << "Deconstructing wire\n";
    }
};

class Network : public Subject<NetworkUpdateEvent> {
public:
    glm::vec3 hsvColor;
    Network();
    explicit Network(glm::vec3 hsvColor);

    std::list<Wire*> wires;
    std::list<Joint*> joints;

    std::pair<Joint*, Pin> parentPin{};
    std::unordered_map<Joint*, Pin> childPins;

	Color getColor() const;
    void removeWire(Wire* wire, bool disconnect); // jointVertexData are only deleted if they have no more wires
    void removeJoint(Joint* joint); // We have to pass
    static void connect(Wire* wire);

    static void connect(Sim::Simulation* sim, const Pin& parent, const Pin& child);
    static void disconnect(Sim::Simulation* sim, const Pin& parent, const Pin& child);

    void update();

    ~Network() override {
        std::cout << "Deconstructing network " << this << "\n";
    }
};


#endif //BUILDIT_WIRE_H
