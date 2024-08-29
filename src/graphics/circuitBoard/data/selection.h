//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include <memory>
#include "graphics/circuitBoard/components/wires/wire.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"

const glm::vec3 SELECTED_COLOR{200, 100, 0};

class Selection {
private:
    JointContainer *jointContainer;
    WireContainer *wireContainer;
    WiresRenderer *renderer;
public:
    Selection(JointContainer *jointContainer, WireContainer *wireContainer, WiresRenderer* renderer)
        : jointContainer(jointContainer), wireContainer(wireContainer), renderer(renderer) {};
    std::unordered_set<const Joint*> joints;
    std::unordered_set<const Wire*> wires;
    std::unordered_set<const Node*> nodes;
    void clear();
    void addJoint(const Joint *joint);
    void removeJoint(const Joint *joint);
    void addWire(const Wire *wire);
    void removeWire(const Wire *wire);
    void addNode(const Node *node);
    void removeNode(const Node *node);
};


#endif //BUILDIT_SELECTION_H
