//
// Created by felix on 7/31/24.
//

#include "simulation.h"
#include "simulation/node.h"
#include <thread>
#include <iostream>
#include <algorithm>

[[noreturn]] void Sim::Simulation::simulate() {
    std::cout << "Starting Simulation...\n";
    this->simStart = std::chrono::high_resolution_clock::now();
    while (true) {
        while (!this->updateQueue.empty()) {
            Sim::update(&updateQueue, updateQueue.front());
            updateQueue.pop();
            this->updates++;
            this->upsCalcUpdates++;
            modifyLock.unlock();
            if (targetUPS != 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds((int) (1000 / targetUPS)));
            }
        }
        std::unique_lock<std::mutex> lock(this->updateLock);
        this->updateCondition.wait(lock, [this]{return !this->updateQueue.empty();});
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

void Sim::Simulation::connect(Reference parent, Reference child, bool update) {
    this->modifyLock.lock();
    // Add child to parents children
    parent.node->children[parent.index].emplace_back(child);
    // Add parent to children parents
    child.node->parents[child.index] = parent;
    if (update) {
        child.node->setInput(child.index, parent.node->getOutput(parent.index));
        {
            std::unique_lock<std::mutex> lock(this->updateLock);
            this->updateQueue.push(child.node);
        }
        this->updateCondition.notify_one();
    }
    this->modifyLock.unlock();
}

void Sim::Simulation::disconnect(Reference parent, Reference child) {
    this->modifyLock.lock();
    // Remove child from parents children
    std::vector<Reference>* references = &parent.node->children[parent.index];
    references->erase(std::find_if(references->begin(), references->end(), [&child](const Reference& ref){
        return ref.node == child.node;
    }));
    // Remove parent from children parents
    child.node->parents[child.index] = {};
    {
        std::unique_lock<std::mutex> lock(this->updateLock);
        this->updateQueue.push(child.node);
    }
    this->updateCondition.notify_one();
    this->modifyLock.unlock();
}