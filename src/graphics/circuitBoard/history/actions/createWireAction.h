//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_CREATEWIREACTION_H
#define BUILDIT_CREATEWIREACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"
#include "graphics/circuitBoard/components/nodes/node.h"

class CreateWireAction : public Action {
private:
    Sim::Simulation *simulation;
    WireContainer *wireContainer;
    NetworkContainer *networkContainer;
    WiresRenderer *wiresRenderer;
    std::shared_ptr<Wire> wire;
    std::shared_ptr<Network> deletedNetwork;
public:
    CreateWireAction(Sim::Simulation *simulation, WireContainer *wireContainer, NetworkContainer *networkContainer, WiresRenderer *renderer, const std::shared_ptr<Wire>& wire, bool reversed)
    : wire(wire), simulation(simulation), wiresRenderer(renderer), networkContainer(networkContainer), wireContainer(wireContainer), Action(reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEWIREACTION_H
