//
// Created by felix on 9/10/24.
//

#ifndef BUILDIT_NOTGATE_H
#define BUILDIT_NOTGATE_H


#include "gate.h"

class NotGate : public Gate {
public:
    void visit(Visitor *visitor) override;
};


#endif //BUILDIT_NOTGATE_H
