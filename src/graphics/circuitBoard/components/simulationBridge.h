//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_SIMULATIONBRIDGE_H
#define BUILDIT_SIMULATIONBRIDGE_H


#include "graphics/circuitBoard/components/wires/wires.h"
#include "graphics/circuitBoard/renderer/node/nodeRenderers.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
#include "graphics/circuitBoard/components/wires/jointContainer.h"

/**
 * Joins wires and Nodes and handles connection and disconnection of simulation Nodes if joints lie above pins
 */
class SimulationBridge : public NodeContainer, public JointContainer {

private:
    Sim::Simulation* simulation;

    Nodes* nodes;
    Wires* wires;

    void checkJoint(Joint* joint, bool disconnect = false);

    void connectChild(Joint* joint, std::unique_ptr<Pin>& childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, std::unique_ptr<Pin>& parentPin);
    void disconnectParent(Joint* joint);
public:
    SimulationBridge(Sim::Simulation* sim, Nodes* nodes, Wires* wires);
    void addNode(const std::shared_ptr<Node>& node) override;
    void removeNode(Node* node) override;
    void moveNode(Node* node, glm::vec2 newPos, bool updateBuffer) override;
    void addJoint(const std::shared_ptr<Joint> &joint) override;
    void removeJoint(Joint* joint) override;
    void moveJoint(Joint* joint, glm::vec2 newPos) override;
};


#endif //BUILDIT_SIMULATIONBRIDGE_H
