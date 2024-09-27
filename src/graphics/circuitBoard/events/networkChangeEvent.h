//
// Created by felix on 9/27/24.
//

#ifndef BUILDIT_NETWORKCHANGEEVENT_H
#define BUILDIT_NETWORKCHANGEEVENT_H

class Networkable;
class Network;

struct NetworkChangeEvent {
    Networkable *networkable;
    Network *newNetwork;
    bool before = false;
};

#endif //BUILDIT_NETWORKCHANGEEVENT_H
