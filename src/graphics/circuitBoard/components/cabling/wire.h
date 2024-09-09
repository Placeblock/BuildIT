//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRE_H
#define BUILDIT_WIRE_H

#include <vector>
#include <memory>
#include <iostream>

#include "pin.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"

class Wire;
class Network;
class Joint;
class Networkable;

struct NetworkChangeEvent {
    Networkable *networkable;
    Network *newNetwork;
    bool before = false;
};

class Networkable : public Subject<NetworkChangeEvent> {
private:
    Network *network = nullptr;
public:
    Networkable() = default;
    explicit Networkable(Network *network);
    Network* getNetwork();
    void setNetwork(Network *newNetwork);
};

struct NetworkUpdateEvent {};

class Joint : public Networkable, public Component, public Movable {
public:
    std::set<Wire*> wires;
    Pin pin{};

    explicit Joint(glm::vec2 pos);
    Joint(glm::vec2 pos, Network* network);

    [[nodiscard]] Wire* getWire(Joint* other) const;

    void visit(Visitor *visitor) override;

    ~Joint() override;
};

class Wire : public Networkable {
public:
    Wire(Joint* start, Joint* end);
    Wire(Joint* start, Joint* end, Network* network);
    Joint* start = nullptr;
    Joint* end = nullptr;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;

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
