//
// Created by felix on 7/31/24.
//

#ifndef BUILDIT_SIMULATION_H
#define BUILDIT_SIMULATION_H

#include <queue>
#include <set>
#include <condition_variable>
#include <memory>
#include <mutex>

namespace Sim {
    class Node;
    class Reference;

    class Simulation {
    private:
        std::set<std::shared_ptr<Node>> nodes;
        std::queue<Node*> updateQueue;
        std::mutex updateLock;
        std::condition_variable updateCondition;
    public:
        std::mutex modifyLock;
        int targetUPS = 0;
        float currentUPS = 0; // AVERAGE OF LAST SECOND
        std::chrono::time_point<std::chrono::high_resolution_clock> simStart;
        long updates = 0;
        long upsCalcUpdates = 0;

        [[noreturn]] void simulate();
        [[noreturn]] void measure();
        void addNode(std::shared_ptr<Sim::Node> node);
        void removeNode(std::shared_ptr<Sim::Node> node);
        void connect(Reference parent, Reference child, bool update);
        void disconnect(Reference parent, Reference child);
    };
}


#endif //BUILDIT_SIMULATION_H
