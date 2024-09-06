//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_WIREEVENTS_H
#define BUILDIT_WIREEVENTS_H

#include "wire.h"

struct WireAddEvent {
    Wire *joint;
};

struct WireRemoveEvent {
    Wire *joint;
};

#endif //BUILDIT_WIREEVENTS_H
