//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_WIRE_H
#define BUILDIT_WIRE_H

#include <iostream>
#include <set>
#include <list>

#include "glm/vec3.hpp"
#include "graphics/types.h"
#include "pin.h"
#include "graphics/circuitBoard/components/abstraction/movable.h"
#include "graphics/circuitBoard/components/abstraction/circleInteractable.h"
#include "graphics/circuitBoard/components/abstraction/lineInteractable.h"
#include "graphics/circuitBoard/selection/selectable.h"
#include "simulation/simulation.h"
#include "graphics/circuitBoard/events/networkChangeEvent.h"

class Networkable : public Subject<NetworkChangeEvent> {
    std::shared_ptr<Network> network;
public:
    Networkable() = default;
    explicit Networkable(std::shared_ptr<Network> network);
    std::shared_ptr<Network> getNetwork() const;
    void setNetwork(std::shared_ptr<Network> network);
};

struct NetworkUpdateEvent {
    Network *network;
};

class Joint final : public Networkable, public Movable, public Selectable, public CircleInteractable {
private:
    glm::vec2 pos;
protected:
    [[nodiscard]] glm::vec2 getCenter() const override;
public:
    std::set<Wire*> wires;
    Pin pin{};

    explicit Joint(glm::vec2 pos);
    Joint(glm::vec2 pos, std::shared_ptr<Network> network);
    Joint(Joint& other);

    [[nodiscard]] Wire* getWire(Joint* other) const;
    void onMove(glm::vec2 delta) override;

    [[nodiscard]] glm::vec2 getPos() const;
    [[nodiscard]] Color getColor() const;

    void visit(Visitor *visitor) override;
};

class Wire final : public Networkable, public Movable, public Selectable, public LineInteractable {
protected:
    [[nodiscard]] glm::vec2 getStartPos() const override;
    [[nodiscard]] glm::vec2 getEndPos() const override;
public:
    Wire(Joint* start, Joint* end);
    Wire(Joint* start, Joint* end, std::shared_ptr<Network> network);
    Wire(Wire& other);

    Joint* start = nullptr;
    Joint* end = nullptr;
    [[nodiscard]] Joint* getOther(const Joint* cell) const;

	void connect();
	void disconnect();

    [[nodiscard]] Color getColor() const;

    void visit(Visitor *visitor) override;
};

class Network final : public Subject<NetworkUpdateEvent> {
public:
    glm::vec3 hsvColor;
    Color renderedColor;
    Network();
    explicit Network(glm::vec3 hsvColor);

    std::list<Wire*> wires;
    std::list<Joint*> joints;

    std::pair<Joint*, Pin> parentPin{};
    std::unordered_map<Joint*, Pin> childPins;

	Color getRenderedColor() const;
    void removeWire(Wire* wire);
    void removeJoint(Joint* joint); // We have to pass

    static void connect(Sim::Simulation* sim, const Pin& parent, const Pin& child);
    static void disconnect(Sim::Simulation* sim, const Pin& parent, const Pin& child);

    void update();
};


#endif //BUILDIT_WIRE_H
