//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_CREATEWIREACTION_H
#define BUILDIT_CREATEWIREACTION_H


#include "graphics/history/wiresAction.h"

class CreateWireAction : public WiresAction {
private:
    std::shared_ptr<Wire> wire;
    std::shared_ptr<Network> deletedNetwork;
public:
    CreateWireAction(const std::shared_ptr<Wire>& wire, Wires* wires, WiresRenderer* renderer, bool reversed)
    : wire(wire), WiresAction(wires, renderer, reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_CREATEWIREACTION_H
