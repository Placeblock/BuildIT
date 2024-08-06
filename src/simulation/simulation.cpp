//
// Created by felix on 7/31/24.
//

#include <thread>
#include <iostream>
#include "simulation.h"

[[noreturn]] void Sim::Simulation::simulate() {
    this->simStart = std::chrono::high_resolution_clock::now();
    while (true) {
        while (!this->updateQueue.empty()) {
            Sim::update(&updateQueue, updateQueue.front());
            updateQueue.pop();
            this->updates++;
            this->upsCalcUpdates++;
            if (targetUPS != 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds((int) (1000 / targetUPS)));
            }
        }
    }
}

void Sim::Simulation::measure() {
    while (true) {
        this->currentUPS = (float)this->upsCalcUpdates;
        this->upsCalcUpdates = 0;
        std::this_thread::sleep_for(std::chrono::seconds (1));
    }
}

void Sim::Simulation::addNode(Sim::Node *node) {
    this->nodes.emplace_back(node);
    this->updateQueue.push(node);
}