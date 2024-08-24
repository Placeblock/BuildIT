//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_CREATEWIREACTION_H
#define BUILDIT_CREATEWIREACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"
#include "graphics/circuitBoard/elements/nodes/node.h"

class CreateWireAction : public WiresAction {
private:
    Sim::Simulation* simulation;
    std::shared_ptr<Wire> wire;
    std::shared_ptr<Network> deletedNetwork;
public:
    CreateWireAction(const std::shared_ptr<Wire>& wire, Wires* wires, WiresRenderer* renderer, Sim::Simulation* simulation, bool reversed)
    : wire(wire), simulation(simulation), WiresAction(wires, renderer, reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEWIREACTION_H
