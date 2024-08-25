//
// Created by felix on 8/22/24.
//

#ifndef BUILDIT_NODEACTION_H
#define BUILDIT_NODEACTION_H


#include "action.h"
#include "graphics/circuitBoard/elements/nodes/node.h"

class NodeAction : public Action {
public:
    NodeAction(Nodes* nodes, bool reversed)
    : nodes(nodes), Action(reversed) {};
protected:
    Nodes *nodes;
};


#endif //BUILDIT_NODEACTION_H
