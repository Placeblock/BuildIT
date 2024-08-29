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
 * Ties wires and Nodes together and handles synchronization of simulation Nodes if nodes or joints are moved around, removed and added
 */
class SimulationBridge : public NodeContainer, public JointContainer {
private:
    Sim::Simulation* simulation;

    void checkNode(Node* node, bool disconnect = false);
    void checkJoint(Joint* joint, bool disconnect = false);

    void connectChild(Joint* joint, Pin childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, Pin parentPin);
    void disconnectParent(Joint* joint);
public:
    Nodes* nodes;
    Wires* wires;

    SimulationBridge(Sim::Simulation* sim, Nodes* nodes, Wires* wires);
    void addNode(const std::shared_ptr<Node>& node) override;
    void removeNode(Node* node) override;
    void moveNode(Node* node, glm::vec2 newPos, bool updateBuffer) override;
    void addJoint(const std::shared_ptr<Joint> &joint) override;
    void removeJoint(Joint* joint) override;
    void moveJoint(Joint* joint, glm::vec2 newPos) override;

    size_t getJointIndex(const Joint* joint) const override {
        return this->wires->getJointIndex(joint);
    }
    [[nodiscard]] std::set<const Joint*> getJoints() const override {
        return this->wires->getJoints();
    }
    void setNetwork(Joint *joint, Network *network) override {
        this->wires->setNetwork(joint, network);
    }
    std::shared_ptr<Joint> getOwningRef(const Joint *joint) const override {
        return this->wires->getOwningRef(joint);
    }
};


#endif //BUILDIT_SIMULATIONBRIDGE_H
