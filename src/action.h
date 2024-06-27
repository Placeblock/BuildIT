#ifndef BUILDIT_ACTION_H
#define BUILDIT_ACTION_H

#include "algorithm/algorithm.h"
#include "selection.h"

class Action {
public:
    virtual void execute() = 0;
    virtual void rewind() = 0;
};

class AddInputAction : public Action {
public:
    Node* gate;
};

class AddOutputAction : public Action {
public:
    Node* gate;
};

class ConnectAction : public Action {
public:
    Node* parent;
    Node* child;
};

class MoveAction : public Action {
    Selection selection;
};

class AddGateAction : public Action {
    Node* gate;
};

class DeleteAction : public Action {
    Selection selection;
};

#endif //BUILDIT_ACTION_H
