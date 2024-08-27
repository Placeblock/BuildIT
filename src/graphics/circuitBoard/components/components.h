//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_COMPONENTS_H
#define BUILDIT_COMPONENTS_H


#include "graphics/circuitBoard/components/nodes/node.h"
#include "graphics/circuitBoard/components/wires/wires.h"
#include "graphics/circuitBoard/renderer/node/nodeRenderers.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"

class Components {

private:
    Nodes nodes;
    NodeRenderers nodeRenderers;
    Wires wires;
    WiresRenderer wiresRenderer;

public:
    void addNode(Node* node);
    void removeNode(Node* node);
    void moveNode(Node* node, glm::vec2 newPos);
    void addJoint(Joint* joint);
    void removeJoint(Joint* joint);
    void moveJoint(Joint* joint, glm::vec2 newPos);
};


#endif //BUILDIT_COMPONENTS_H
