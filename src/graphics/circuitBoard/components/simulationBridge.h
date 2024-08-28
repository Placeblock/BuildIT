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
    Nodes nodes;
    NodeRenderers nodeRenderers;
    Wires wires;
    WiresRenderer wiresRenderer;

public:
    void addNode(const std::shared_ptr<Node>& node);
    void removeNode(Node* node);
    void moveNode(Node* node, glm::vec2 newPos, bool updateBuffer);
    void addJoint(const std::shared_ptr<Joint> &joint);
    void removeJoint(Joint* joint);
    void moveJoint(Joint* joint, glm::vec2 newPos);
    void checkJoint(Joint* joint, bool disconnect = false);
};


#endif //BUILDIT_SIMULATIONBRIDGE_H
