//
// Created by felix on 8/2/24.
//

#ifndef BUILDIT_ACTION_H
#define BUILDIT_ACTION_H

#include "glm/vec2.hpp"
#include <memory>

class Action {
public:
    explicit Action(bool reversed) : reversed(reversed) {};
    bool reversed;
    virtual void execute(bool lastInBatch) = 0;
    virtual void rewind(bool lastInBatch) = 0;
    static void execute(Action* action, bool lastInBatch);
    static void rewind(Action* action, bool lastInBatch);
};


#endif //BUILDIT_ACTION_H
