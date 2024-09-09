//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_VISITOR_H
#define BUILDIT_VISITOR_H

class Gate;
class Joint;

class Visitor {
public:
    virtual void doFor(Gate *vehicle) {};
    virtual void doFor(Joint *vehicle) {};
};

class Visitable {
public:
    virtual void visit(Visitor *visitor) = 0;
};

#endif //BUILDIT_VISITOR_H
