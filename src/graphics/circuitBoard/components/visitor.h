//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_VISITOR_H
#define BUILDIT_VISITOR_H

class Component;
class Positionable;
class Movable;
class Rotatable;
class Node;
class Gate;
class Joint;

class Visitor {
public:
    virtual void doFor(Component *vehicle) {};
    virtual void doFor(Positionable *vehicle) {};
    virtual void doFor(Movable *vehicle) {};
    virtual void doFor(Rotatable *vehicle) {};
    virtual void doFor(Node *vehicle) {};
    virtual void doFor(Gate *vehicle) {};
    virtual void doFor(Joint *vehicle) {};
};

class Visitable {
public:
    virtual void visit(Visitor *visitor) = 0;
};

#endif //BUILDIT_VISITOR_H
