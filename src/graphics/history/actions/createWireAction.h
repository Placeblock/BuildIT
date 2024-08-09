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
    CreateWireAction(std::shared_ptr<Wire> wire, Wires* wires, WiresRenderer* renderer, bool regenerate, bool reversed)
    : wire(std::move(wire)), WiresAction(wires, renderer, regenerate, reversed) {};
    void execute() override;
    void rewind() override;
};


#endif //BUILDIT_CREATEWIREACTION_H
