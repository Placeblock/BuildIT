//
// Created by felix on 9/10/24.
//

#ifndef BUILDIT_NOTGATE_H
#define BUILDIT_NOTGATE_H


#include "gate.h"

class NotGate : public Gate {
public:
    NotGate(glm::vec2 cell, std::string text, const std::shared_ptr<Sim::Node>& simNode);
    void visit(Visitor *visitor) override;
};


#endif //BUILDIT_NOTGATE_H
