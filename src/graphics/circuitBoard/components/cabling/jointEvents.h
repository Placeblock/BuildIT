//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_JOINTEVENTS_H
#define BUILDIT_JOINTEVENTS_H

#include "wire.h"

struct JointAddEvent {
    Joint *joint;
};

struct JointRemoveEvent {
    Joint *joint;
};

#endif //BUILDIT_JOINTEVENTS_H
