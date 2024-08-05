//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_ACTION_H
#define BUILDIT_ACTION_H


#include "wires/wires.h"
#include "renderer/wiresRenderer.h"

class Action {
public:
    bool reversed = false;
    virtual void Execute(Wires* cables, WiresRenderer* renderer, bool regenerate) = 0;
    virtual void Rewind(Wires* cables, WiresRenderer* renderer, bool regenerate) = 0;
    void checkRegenerate(Wires* cables, WiresRenderer* renderer, bool regenerate);
};


#endif //BUILDIT_ACTION_H
