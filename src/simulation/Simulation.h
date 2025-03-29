//
// Created by felix on 29.03.25.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include <memory>
#include <queue>

#include "Node.h"

namespace Sim {
    class Simulation {
        std::queue<std::weak_ptr<Node> > updateQueue;

    public:
        void pollAndUpdate();
    };
}


#endif //SIMULATION_H
