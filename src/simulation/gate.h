//
// Created by felix on 16.07.24.
//

#ifndef BUILDIT_GATE_H
#define BUILDIT_GATE_H

#include "node.h"

namespace Simulation {
    class NotGate : public Node {
    public:
        NotGate() : Node(1, 1) {}
        void update() override;
    };
}


#endif //BUILDIT_GATE_H
