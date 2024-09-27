//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_NETWORKEVENTS_H
#define BUILDIT_NETWORKEVENTS_H

#include <unordered_set>

class Network;
class Networkable;

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

struct NetworkUpdateEvent {
    Network *network;
};

struct NetworkChangeEvent {
    Networkable *networkable;
    Network *newNetwork;
    bool before = false;
};

#endif //BUILDIT_NETWORKEVENTS_H
