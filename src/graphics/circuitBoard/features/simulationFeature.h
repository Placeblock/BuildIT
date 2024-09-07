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
class SimulationFeature : public TypedObserver<ComponentAddEvent, Joint>, public TypedObserver<ComponentRemoveEvent, Joint>,
                          public TypedObserver<ComponentAddEvent, Node>, public TypedObserver<ComponentRemoveEvent, Node>,
                          public CastedObserver<MoveEvent, Joint>, public CastedObserver<MoveEvent, Node>,
                          public CastedObserver<RotateEvent, Node>,
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

    void notify(TypedSubject<ComponentAddEvent, Joint> *subject, const ComponentAddEvent& data) override;
    void notify(TypedSubject<ComponentRemoveEvent, Joint> *subject, const ComponentRemoveEvent& data) override;
    void notify(TypedSubject<ComponentAddEvent, Node> *subject, const ComponentAddEvent& data) override;
    void notify(TypedSubject<ComponentRemoveEvent, Node> *subject, const ComponentRemoveEvent& data) override;
    void notify(Joint *joint, const MoveEvent& data) override;
    void notify(Node *node, const MoveEvent& data) override;
    void notify(Node *node, const RotateEvent& data) override;
};


#endif //BUILDIT_SIMULATIONFEATURE_H
