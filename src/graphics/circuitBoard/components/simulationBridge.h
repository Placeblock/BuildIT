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
#include "graphics/circuitBoard/components/nodes/nodePins.h"

/**
 * Ties wires and nodes together and handles synchronization of simulation nodes if nodes or joints are moved around, removed and added
 */
class SimulationBridge : public Observer<JointAddEvent>, public Observer<JointRemoveEvent>,
        public Observer<NodeAddEvent>, public Observer<NodeRemoveEvent>,
         public MultiObserver<MoveEvent<Joint>, Joint*>, public MultiObserver<MoveEvent<Node>, Node*>,
         public MultiObserver<RotateEvent<Node>, Node*>{

private:
    Sim::Simulation *simulation;
    NodePinHandler *pinHandler;
    Subject<JointAddEvent> *jointAddSubject;
    Subject<JointRemoveEvent> *jointRemoveSubject;

    void checkNode(Node* node, glm::vec2 nodePos, bool disconnect = false);
    void checkJoint(Joint* joint, glm::vec2 jointPos, bool disconnect = false);

    void connectChild(Joint* joint, Pin childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, Pin parentPin);
    void disconnectParent(Joint* joint);
public:
    SimulationBridge(Sim::Simulation *sim, NodePinHandler *pinHandler,
                     Subject<JointAddEvent> *jointAddSubject, Subject<JointRemoveEvent> *jointRemoveSubject);

    void update(const JointAddEvent& data) override;
    void update(const JointRemoveEvent& data) override;
    void update(const NodeAddEvent& data) override;
    void update(const NodeRemoveEvent& data) override;
    void update(const MoveEvent<Node>& data, Node *node) override;
    void update(const MoveEvent<Joint>& data, Joint *joint) override;
    void update(const RotateEvent<Node>& data, Node *node) override;

    ~SimulationBridge() override;
};


#endif //BUILDIT_SIMULATIONBRIDGE_H
