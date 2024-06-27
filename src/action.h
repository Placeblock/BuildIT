#ifndef BUILDIT_ACTION_H
#define BUILDIT_ACTION_H

#include "gate.h"
#include "selection.h"

class Action {
public:
    virtual void execute() = 0;
    virtual void rewind() = 0;
};

class AddInputAction : public Action {
public:
    Gate* gate;
};

class AddOutputAction : public Action {
public:
    Gate* gate;
};

class ConnectAction : public Action {
public:
    Gate* parent;
    Gate* child;
};

class MoveAction : public Action {
    Selection selection;
};

class AddGateAction : public Action {
    Gate* gate;
};

class DeleteAction : public Action {
    Selection selection;
};

#endif //BUILDIT_ACTION_H
