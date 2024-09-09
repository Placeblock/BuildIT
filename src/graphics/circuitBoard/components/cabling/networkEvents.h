//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_NETWORKEVENTS_H
#define BUILDIT_NETWORKEVENTS_H

#include "wire.h"
#include "networkResolver.h"

struct NetworkAddEvent {
    Network *network;
};

struct NetworkRemoveEvent {
    Network *network;
};

struct NetworksMergeEvent {
    Network *persisting;
    Network *deleted;
};

struct NetworksSplitEvent {
    Network *old;
    std::unordered_set<Network*> splitted;
};

#endif //BUILDIT_NETWORKEVENTS_H
