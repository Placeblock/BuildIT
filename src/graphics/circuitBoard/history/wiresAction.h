//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_WIRESACTION_H
#define BUILDIT_WIRESACTION_H


#include "action.h"
#include "graphics/circuitBoard/elements/wires/wires.h"
#include "graphics/circuitBoard/renderer/wiresRenderer.h"

class WiresAction : public Action {
public:
    WiresAction(Wires* wires, WiresRenderer* renderer, bool reversed)
        : wires(wires), renderer(renderer), Action(reversed) {};
    void execute(bool isLastInBatch) override = 0;
    void rewind(bool isLastInBatch) override = 0;
protected:
    Wires *wires;
    WiresRenderer *renderer;
    void regenerate();
};


#endif //BUILDIT_WIRESACTION_H
