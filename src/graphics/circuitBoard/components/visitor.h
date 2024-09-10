//
// Created by felix on 9/9/24.
//

#ifndef BUILDIT_VISITOR_H
#define BUILDIT_VISITOR_H

class NotGate;
class Joint;

class Visitor {
public:
    virtual void doFor(NotGate *notGate) {};
    virtual void doFor(Joint *joint) {};
};

class Visitable {
public:
    virtual void visit(Visitor *visitor) = 0;
};

#endif //BUILDIT_VISITOR_H
