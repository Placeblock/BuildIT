//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_NETWORKEVENTS_H
#define BUILDIT_NETWORKEVENTS_H

#include "wire.h"

struct NetworkAddEvent {
    Network *network;
};

struct NetworkRemoveEvent {
    Network *network;
};

#endif //BUILDIT_NETWORKEVENTS_H
