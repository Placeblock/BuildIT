//
// Created by felix on 29.03.25.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include <queue>

#include "node.hpp"

namespace sim {
class simulation_t {
    std::queue<node_t *> update_queue;
    std::unordered_set<node_t *> updated_nodes;

public:
    void poll_and_update();

    void update(node_t *node);

    [[nodiscard]] bool is_empty() const;
};
}


#endif //SIMULATION_H
