#include <iostream>
#include <queue>
#include <chrono>
#include <bitset>
#include "algorithm.h"

std::queue<Gate*> updateQueue;

void update(struct Gate *gate) {
    //std::cout << "Updating: Gate: " << gate->getName() << "\n";
    // Copying old output values for checking them later
    uint32_t oldOutput = gate->output;
    //std::cout << "Old output: " << std::bitset<32>(oldOutput) << "\n";
    //std::cout << "Current Input: " << std::bitset<32>(gate->input) << "\n";
    // Update the gate
    gate->update();
    //std::cout << "New output: " << std::bitset<32>(gate->output) << "\n";
    // Update children of changed outputs
    for (const auto &output: gate->outputs) {
        if ((oldOutput ^ gate->output) & (1 << output->index)) {
            for (const auto &child: output->children) {
                child->set(output->get());
                updateQueue.push(child->gate);
            }
        }
    }
}

int main() {
    NotGate notGate;
    notGate.addInput(0);
    notGate.addOutput(0);

    notGate.connect(&notGate, 0, 0);
    updateQueue.push(&notGate);

    int updates = 0;
    auto start = std::chrono::steady_clock::now();
    Gate *update_gate;
    while (!updateQueue.empty()) {
        update_gate = updateQueue.front();
        update(update_gate);
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
