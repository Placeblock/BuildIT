//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_SIMULATIONBRIDGE_H
#define BUILDIT_SIMULATIONBRIDGE_H


#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/circuitBoard/components/wires/wires.h"
#include "graphics/circuitBoard/renderer/node/nodeRenderers.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"

/**
 * Joins wires and nodes and handles connection and disconnection of simulation nodes if joints lie above pins
 */
class SimulationBridge {

private:
    Sim::Simulation* simulation;

    Nodes* nodes;
    Wires* wires;

public:
    SimulationBridge(Sim::Simulation* sim, Nodes* nodes, Wires* wires);
    void addNode(const std::shared_ptr<Node>& node);
    void removeNode(Node* node);
    void moveNode(Node* node, glm::vec2 newPos, bool updateBuffer);
    void addJoint(const std::shared_ptr<Joint> &joint);
    void removeJoint(Joint* joint);
    void moveJoint(Joint* joint, glm::vec2 newPos);
    void checkJoint(Joint* joint, bool disconnect = false);

    void connectChild(Joint* joint, std::unique_ptr<Pin>& childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, std::unique_ptr<Pin>& parentPin);
    void disconnectParent(Joint* joint);
};


#endif //BUILDIT_SIMULATIONBRIDGE_H
