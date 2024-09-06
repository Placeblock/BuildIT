//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_SIMULATIONBRIDGE_H
#define BUILDIT_SIMULATIONBRIDGE_H


#include "graphics/circuitBoard/components/wires/wires.h"
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
#include "graphics/circuitBoard/components/wires/jointContainer.h"

/**
 * Ties wires and NodeInteractionManager together and handles synchronization of simulation nodes if nodes or joints are moved around, removed and added
 */
class SimulationBridge {
private:
    Sim::Simulation* simulation;
    WiresRenderer *wiresRenderer;

    void checkNode(Node* node, bool disconnect = false);
    void checkJoint(Joint* joint, bool disconnect = false);

    void connectChild(Joint* joint, Pin childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, Pin parentPin);
    void disconnectParent(Joint* joint);
public:
    NodeInteractionManager* nodes;
    Cabling* wires;

    SimulationBridge(Sim::Simulation* sim, NodeInteractionManager* nodes, Cabling* wires, WiresRenderer *wiresRenderer);
    void addNode(const std::shared_ptr<Node>& node) override;
    void removeNode(Node* node) override;
    void moveNode(Node* node, glm::vec2 newPos, bool updateBuffer) override;
    void addJoint(const std::shared_ptr<Joint> &joint) override;
    void removeJoint(Joint* joint) override;
    void moveJoint(Joint* joint, glm::vec2 newPos) override;

};


#endif //BUILDIT_SIMULATIONBRIDGE_H
