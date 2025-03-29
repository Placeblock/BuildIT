//
// Created by felix on 29.03.25.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include <memory>
#include <queue>

#include "Graph.h"
#include "Node.h"

namespace Sim {
    class Simulation {
        std::queue<std::weak_ptr<Node> > updateQueue;
        std::unique_ptr<Graph> graph;

    public:
        explicit Simulation(std::unique_ptr<Graph> &graph);

        void pollAndUpdate();

        void update(const std::weak_ptr<Node> &node);

        [[nodiscard]] bool isEmpty() const;
    };
}


#endif //SIMULATION_H
