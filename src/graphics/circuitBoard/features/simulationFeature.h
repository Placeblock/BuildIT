//
// Created by felix on 8/27/24.
//

#ifndef BUILDIT_SIMULATIONFEATURE_H
#define BUILDIT_SIMULATIONFEATURE_H


#include "graphics/circuitBoard/components/cabling/cabling.h"
#include "graphics/circuitBoard/components/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/components/nodes/nodePins.h"
#include "feature.h"
#include "graphics/circuitBoard/components/componentContainer.h"

/**
 * Ties wires and nodes together and handles synchronization of simulation nodes if nodes or joints are moved around, removed and added
 */
class SimulationFeature final : public Observer<ComponentAddEvent>, public Observer<ComponentRemoveEvent>,
                                public Observer<MoveEvent>, public Observer<RotateEvent>,
                                public Observer<NetworksMergeEvent>, public Observer<NetworksSplitEvent>,
                                public Feature {
    Sim::Simulation *simulation;
    NodePinHandler *pinHandler;
    Cabling *cabling;

    void checkNode(Node* node, glm::vec2 nodePos, bool disconnect = false);
    void checkJoint(Joint* joint, glm::vec2 jointPos, bool disconnect = false);

    void connectChild(Joint* joint, Pin childPin) const;
    void disconnectChild(Joint* joint) const;
    void connectParent(Joint *joint, Pin parentPin) const;
    void disconnectParent(Joint* joint) const;
public:
    SimulationFeature(Sim::Simulation *sim, NodePinHandler *pinHandler, Cabling *cabling);

    void notify(const ComponentAddEvent& data) override;
    void notify(const ComponentRemoveEvent& data) override;
    void notify(const MoveEvent& data) override;
    void notify(const RotateEvent& data) override;
    void notify(const NetworksMergeEvent& data) override;
    void notify(const NetworksSplitEvent& data) override;
};


#endif //BUILDIT_SIMULATIONFEATURE_H
