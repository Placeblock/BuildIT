//
// Created by felix on 9/7/24.
//

#ifndef BUILDIT_NODEEVENTS_H
#define BUILDIT_NODEEVENTS_H


#include "node.h"

struct NodeAddEvent {
    Node *node;
};

struct NodeRemoveEvent {
    Node *node;
};

#endif //BUILDIT_NODEEVENTS_H
