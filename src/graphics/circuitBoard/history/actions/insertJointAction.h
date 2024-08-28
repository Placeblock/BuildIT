//
// Created by felix on 8/3/24.
//

#ifndef BUILDIT_INSERTJOINTACTION_H
#define BUILDIT_INSERTJOINTACTION_H


#include "graphics/circuitBoard/history/wiresAction.h"

class InsertJointAction : public WiresAction {
private:
    std::shared_ptr<Joint> joint;
    std::shared_ptr<Wire> splitWire;
    std::shared_ptr<Wire> createdWires[2];
public:
    InsertJointAction(const std::shared_ptr<Joint>& joint, Wires* wires, WiresRenderer* renderer, bool reversed)
        : joint(joint), WiresAction(wires, renderer, reversed) {};
    void execute(bool lastInBatch) override;
    void rewind(bool lastInBatch) override;
};


#endif //BUILDIT_INSERTJOINTACTION_H
