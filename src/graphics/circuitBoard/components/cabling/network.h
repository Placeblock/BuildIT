//
// Created by felix on 9/29/24.
//

#ifndef BUILDIT_NETWORK_H
#define BUILDIT_NETWORK_H

#include <list>
#include "glm/vec3.hpp"
#include "graphics/types.h"
#include "pin.h"
#include "simulation/simulation.h"
#include "graphics/circuitBoard/events/networkEvents.h"
#include "graphics/circuitBoard/observer.h"

class Wire;
class Joint;

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

class Networkable : public Subject<NetworkChangeEvent> {
    std::shared_ptr<Network> network;
public:
    Networkable() = default;
    explicit Networkable(std::shared_ptr<Network> network);
    std::shared_ptr<Network> getNetwork() const;
    void setNetwork(std::shared_ptr<Network> network);
};


#endif //BUILDIT_NETWORK_H
