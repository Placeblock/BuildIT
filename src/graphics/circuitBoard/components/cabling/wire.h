//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRE_H
#define BUILDIT_WIRE_H

#include <iostream>

#include "pin.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/components/abstraction/circleInteractable.h"
#include "graphics/circuitBoard/components/abstraction/lineInteractable.h"

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
    Network *network = nullptr;
public:
    Networkable() = default;
    explicit Networkable(Network *network);
    Network* getNetwork() const;
    void setNetwork(Network *newNetwork);
};

struct NetworkUpdateEvent {
    Network *network;
};

class Joint final : public Networkable, public Movable, public Selectable, public CircleInteractable {
    glm::vec2 pos;
public:
    std::set<Wire*> wires;
    Pin pin{};

    explicit Joint(glm::vec2 pos);
    Joint(glm::vec2 pos, Network* network);

    [[nodiscard]] Wire* getWire(Joint* other) const;
    void onMove(glm::vec2 delta) override;

    [[nodiscard]] glm::vec2 getPos() const;

    void visit(Visitor *visitor) override;

    ~Joint() override;
};

class Wire final : public Networkable, public Movable, public Selectable, public LineInteractable {
public:
    Wire(Joint* start, Joint* end);
    Wire(Joint* start, Joint* end, Network* network);
    Joint* start = nullptr;
    Joint* end = nullptr;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;

	void connect();
	void disconnect();

    void onMove(glm::vec2 delta) override;

    void visit(Visitor *visitor) override;

    ~Wire() override {
        std::cout << "Deconstructing wire\n";
    }
};

class Network final : public Subject<NetworkUpdateEvent> {
public:
    glm::vec3 hsvColor;
    Network();
    explicit Network(glm::vec3 hsvColor);

    std::list<Wire*> wires;
    std::list<Joint*> joints;

    std::pair<Joint*, Pin> parentPin{};
    std::unordered_map<Joint*, Pin> childPins;

	Color getColor() const;
    void removeWire(Wire* wire);
    void removeJoint(Joint* joint); // We have to pass

    static void connect(Sim::Simulation* sim, const Pin& parent, const Pin& child);
    static void disconnect(Sim::Simulation* sim, const Pin& parent, const Pin& child);

    void update();

    ~Network() override {
        std::cout << "Deconstructing network " << this << "\n";
    }
};


#endif //BUILDIT_WIRE_H
