//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_WIRESACTION_H
#define BUILDIT_WIRESACTION_H


#include "action.h"
#include "graphics/elements/wires/wires.h"
#include "graphics/renderer/wiresRenderer.h"

class WiresAction : public Action {
public:
    WiresAction(Wires* wires, WiresRenderer* renderer, bool regenerate, bool reversed)
        : wires(wires), renderer(renderer), regenerate(regenerate), Action(reversed) {};
protected:
    Wires *wires;
    WiresRenderer *renderer;
    bool regenerate;
    void checkRegenerate();
};


#endif //BUILDIT_WIRESACTION_H
