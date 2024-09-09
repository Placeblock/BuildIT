//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_SIMULATIONFEATURE_H
#define BUILDIT_SIMULATIONFEATURE_H


#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/components/nodes/renderer/nodeRenderers.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/components/nodes/nodes.h"
#include "graphics/circuitBoard/components/nodes/nodePins.h"
#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"

/**
 * Ties wires and nodes together and handles synchronization of simulation nodes if nodes or joints are moved around, removed and added
 */
class SimulationFeature : public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>,
                          public Observer<MoveEvent>, public Observer<RotateEvent>,
                          public Observer<NetworksMergeEvent>, public Observer<NetworksSplitEvent>,
                          public Feature {

private:
    Sim::Simulation *simulation;
    NodePinHandler *pinHandler;
    Cabling *cabling;

    void checkNode(Node* node, glm::vec2 nodePos, bool disconnect = false);
    void checkJoint(Joint* joint, glm::vec2 jointPos, bool disconnect = false);

    void connectChild(Joint* joint, Pin childPin);
    void disconnectChild(Joint* joint);
    void connectParent(Joint *joint, Pin parentPin);
    void disconnectParent(Joint* joint);
public:
    SimulationFeature(Sim::Simulation *sim, NodePinHandler *pinHandler, Cabling *cabling);

    void notify(Subject<ComponentAddEvent> *subject, const ComponentAddEvent& data) override;
    void notify(Subject<ComponentRemoveEvent> *subject, const ComponentRemoveEvent& data) override;
    void notify(Subject<MoveEvent> *subject, const MoveEvent& data) override;
    void notify(Subject<RotateEvent> *subject, const RotateEvent& data) override;
    void notify(Subject<NetworksMergeEvent> *subject, const NetworksMergeEvent& data) override;
    void notify(Subject<NetworksSplitEvent> *subject, const NetworksSplitEvent& data) override;
};


#endif //BUILDIT_SIMULATIONFEATURE_H
