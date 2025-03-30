//
// Created by felix on 29.03.25.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include <memory>
#include <queue>

#include "graph.h"
#include "node.h"

namespace Sim {
    class simulation {
        std::queue<node *> updateQueue;
        std::unique_ptr<graph> graph;

    public:
        explicit simulation(std::unique_ptr<graph> &graph);

        void pollAndUpdate();

        void update(node *node);

        [[nodiscard]] bool isEmpty() const;
    };
}


#endif //SIMULATION_H
