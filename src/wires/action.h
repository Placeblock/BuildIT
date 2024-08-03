//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_ACTION_H
#define BUILDIT_ACTION_H


#include "wires.h"

class Action {
public:
    bool reversed = false;
    virtual void Execute(Wires* cables) = 0;
    virtual void Rewind(Wires* cables) = 0;
};


#endif //BUILDIT_ACTION_H
