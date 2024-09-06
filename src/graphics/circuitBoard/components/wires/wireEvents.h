//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_WIREEVENTS_H
#define BUILDIT_WIREEVENTS_H

#include "wire.h"

struct WireAddEvent {
    Wire *wire;
};

struct WireRemoveEvent {
    Wire *wire;
};

#endif //BUILDIT_WIREEVENTS_H
