//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_ACTION_H
#define BUILDIT_ACTION_H


#include "cables.h"

class Action {
public:
    virtual void Execute(Cables* cables) = 0;
    virtual void Rewind(Cables* cables) = 0;
};


#endif //BUILDIT_ACTION_H
