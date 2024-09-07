//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_SIMULATIONFEATURE_H
#define BUILDIT_SIMULATIONFEATURE_H


#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
#include "graphics/circuitBoard/components/cabling/jointContainer.h"
#include "graphics/circuitBoard/components/nodes/nodePins.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "feature.h"

/**
 * Ties wires and nodes together and handles synchronization of simulation nodes if nodes or joints are moved around, removed and added
 */
class SimulationFeature : public Observer<JointAddEvent>, public Observer<JointRemoveEvent>,
                          public Observer<NodeAddEvent>, public Observer<NodeRemoveEvent>,
                          public Observer<MoveEvent<Joint>>, public Observer<MoveEvent<Node>>,
                          public Observer<RotateEvent<Node>>,
                          public Feature {

private:
    Sim::Simulation *simulation;
    NodePinHandler *pinHandler;
    Cabling *cabling;
    NodeContainer *nodes;
    JointContainer *joints;

    void checkNode(Node* node, glm::vec2 nodePos, bool disconnect = false);
    void checkJoint(Joint* joint, glm::vec2 jointPos, bool disconnect = false);

    void connectChild(Joint* joint, Pin childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, Pin parentPin);
    void disconnectParent(Joint* joint);
public:
    SimulationFeature(Sim::Simulation *sim, NodePinHandler *pinHandler, Cabling *cabling,
                      JointContainer *joints, NodeContainer *nodes);

    void update(Subject<JointAddEvent> *subject, const JointAddEvent& data) override;
    void update(Subject<JointRemoveEvent> *subject, const JointRemoveEvent& data) override;
    void update(Subject<NodeAddEvent> *subject, const NodeAddEvent& data) override;
    void update(Subject<NodeRemoveEvent> *subject, const NodeRemoveEvent& data) override;
    void update(Subject<MoveEvent<Node>> *subject, const MoveEvent<Node>& data) override;
    void update(Subject<MoveEvent<Joint>> *subject, const MoveEvent<Joint>& data) override;
    void update(Subject<RotateEvent<Node>> *subject, const RotateEvent<Node>& data) override;
};


#endif //BUILDIT_SIMULATIONFEATURE_H
