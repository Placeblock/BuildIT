#include <iostream>
#include <queue>
#include <chrono>
#include "simulation/gate.h"
#include "graphics/graphics.h"

using namespace Simulation;

std::queue<Node*> updateQueue;

int main() {
    NotGate notGate;
    Simulation::connect(Pin{&notGate, 0}, Pin{&notGate, 0});
    updateQueue.push(&notGate);

    Graphics graphics;
    graphics.start();

    int updates = 0;
    auto start = std::chrono::steady_clock::now();
    Node *update_gate;
    while (!updateQueue.empty()) {
        update_gate = updateQueue.front();
        Simulation::update(&updateQueue, update_gate);
        updateQueue.pop();
        updates++;
        if (updates % 10000000 == 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
            std::cout << "Updates: " << (updates/elapsed) << " U/μs\n";
        }
    }

    return 0;
}
