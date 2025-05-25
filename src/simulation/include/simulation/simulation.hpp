//
// Created by felix on 29.03.25.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include <memory>
#include <queue>

#include "graph.hpp"
#include "node.hpp"

namespace sim {
    class simulation {
        std::queue<node *> update_queue;

    public:
        void poll_and_update();

        void update(node *node);

        [[nodiscard]] bool is_empty() const;
    };
}


#endif //SIMULATION_H
