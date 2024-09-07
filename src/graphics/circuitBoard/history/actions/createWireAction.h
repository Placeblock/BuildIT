//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_CREATEWIREACTION_H
#define BUILDIT_CREATEWIREACTION_H

#include "graphics/circuitBoard/components/cabling/wireContainer.h"
#include "graphics/circuitBoard/components/cabling/jointContainer.h"
#include "graphics/circuitBoard/components/cabling/networkContainer.h"
#include "graphics/circuitBoard/renderer/cablingRenderer.h"
#include "graphics/circuitBoard/history/action.h"

class CreateWireAction : public Action {
private:
    Sim::Simulation *simulation;
    WireContainer *wireContainer;
    NetworkContainer *networkContainer;
    JointContainer *jointContainer;
    std::shared_ptr<Wire> wire;
    std::shared_ptr<Network> deletedNetwork;
public:
    CreateWireAction(Sim::Simulation *simulation, WireContainer *wireContainer, NetworkContainer *networkContainer,
                     JointContainer *jointContainer, const std::shared_ptr<Wire>& wire, bool reversed)
    : wire(wire), simulation(simulation), networkContainer(networkContainer),
      jointContainer(jointContainer), wireContainer(wireContainer), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEWIREACTION_H
