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
    virtual void execute() = 0;
    virtual void rewind() = 0;
};


#endif //BUILDIT_ACTION_H
