//
// Created by felix on 7/31/24.
//

#include <thread>
#include <iostream>
#include "simulation.h"

[[noreturn]] void Sim::Simulation::simulate() {
    std::cout << "Starting Simulation...\n";
    this->simStart = std::chrono::high_resolution_clock::now();
    while (true) {
        while (!this->updateQueue.empty()) {
            modifyLock.lock();
            Sim::update(&updateQueue, updateQueue.front());
            updateQueue.pop();
            this->updates++;
            this->upsCalcUpdates++;
            modifyLock.unlock();
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

void Sim::Simulation::addNode(std::shared_ptr<Sim::Node> node) {
    this->nodes.insert(node);
}

void Sim::Simulation::removeNode(std::shared_ptr<Sim::Node> node) {
    this->nodes.erase(node);
}

void Sim::Simulation::connect(Reference parent, Reference child) {
    this->modifyLock.lock();
    // Add child to parents children
    parent.node->children[parent.index].emplace_back(child);
    // Add parent to children parents
    child.node->parents[child.index] = parent;
    this->updateQueue.push(child.node);
    this->modifyLock.unlock();
}

void Sim::Simulation::disconnect(Reference parent, Reference child) {
    this->modifyLock.lock();
    // Remove child from parents children
    for (auto &pin: parent.node->children[parent.index]) {
        if (pin.targetNode == child.node) {
            pin.node = nullptr;
            pin.targetNode = nullptr;
        }
    }
    // Remove parent from children parents
    child.node->parents[child.index].node = nullptr;
    child.node->parents[child.index].targetNode = nullptr;
    this->updateQueue.push(child.node);
    this->modifyLock.unlock();
}