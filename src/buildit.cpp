#include <iostream>
#include <queue>
#include <chrono>
#include <bitset>
#include "tree.h"

std::queue<Gate*> updateQueue;

void update(struct Gate *gate) {
    //printf("Updating gate\n");
    // Copying old output values for checking them later
    uint32_t oldOutput = gate->output;
    // Update the gate
    gate->update();
    //std::cout << "New output: " << std::bitset<32>(gate->output) << "\n";
    // Update children of changed outputs
    for (const auto &child: gate->children) {
        if ((oldOutput ^ gate->output) & (1 << child.output)) {
            child.reference->gate->setInput(child.reference->index, gate->getOutput(child.output));
            updateQueue.push(child.reference->gate);
        }
    }
}

int main() {
    OnGate onGate;
    onGate.setOutputs(1);
    AndGate andGate;
    andGate.setInputs(2);
    andGate.setOutputs(1);
    NotGate notGate;
    notGate.setInputs(2);
    notGate.setOutputs(2);
    notGate.recalcInputMask();
    notGate.setOutput(0, true);

    andGate.connect(&notGate, 0, 0);
    notGate.connect(&andGate, 0, 0);
    onGate.connect(&andGate, 0, 1);

    updateQueue.push(&onGate);
    updateQueue.push(&andGate);

    int updates = 0;
    auto start = std::chrono::steady_clock::now();
    Gate *update_gate;
    while (!updateQueue.empty()) {
        update_gate = updateQueue.front();
        update(update_gate);
        updateQueue.pop();
        updates++;
        if (updates % 100000000 == 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
            std::cout << "Updates: " << (updates/elapsed) << " U/μs\n";
        }
    }

    return 0;
}
