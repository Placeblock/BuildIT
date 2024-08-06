//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_SIMULATION_H
#define BUILDIT_SIMULATION_H


#include <queue>
#include <chrono>
#include "node.h"

namespace Sim {
    class Simulation {
    private:
        std::vector<std::shared_ptr<Node>> nodes;
        std::queue<std::shared_ptr<Node>> updateQueue;
    public:
        int targetUPS = 0;
        float currentUPS = 0; // AVERAGE OF LAST SECOND
        std::chrono::time_point<std::chrono::high_resolution_clock> simStart;
        long updates = 0;
        long upsCalcUpdates = 0;

        [[noreturn]] void simulate();
        [[noreturn]] void measure();
        void addNode(std::shared_ptr<Sim::Node> node);
    };
}


#endif //BUILDIT_SIMULATION_H