//
// Created by felix on 9/10/24.
//

#ifndef BUILDIT_NOTGATE_H
#define BUILDIT_NOTGATE_H


#include "gate.h"

class NotGate : public Gate {
public:
    NotGate(glm::vec2 pos, const std::shared_ptr<Sim::Node>& simNode);

    NotGate(NotGate& other);
};


#endif //BUILDIT_NOTGATE_H
