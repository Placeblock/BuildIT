//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_CREATEWIREACTION_H
#define BUILDIT_CREATEWIREACTION_H


#include "wires/action.h"

class CreateWireAction : public Action {
private:
    std::shared_ptr<Wire> wire;
    std::shared_ptr<Network> deletedNetwork;
public:
    explicit CreateWireAction(std::shared_ptr<Wire> wire);
    void Execute(Wires* wires, WiresRenderer* renderer, bool regenerate) override;
    void Rewind(Wires* wires, WiresRenderer* renderer, bool regenerate) override;
};


#endif //BUILDIT_CREATEWIREACTION_H
