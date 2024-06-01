#include <iostream>
#include <queue>
#include <chrono>
#include "tree.h"

std::queue<Gate*> updateQueue;
ConnectionManager manager;

void update(struct Gate *gate) {
    // Copying old output values for checking them later
    uint32_t oldOutput = gate->output;
    // Update the gate
    gate->update();
    // Update children of changed outputs
    for (const auto &connection: manager.children[gate]) {
        if ((oldOutput ^ gate->output) & (1 << connection->output)) {
            connection->child->setInput(connection->input, gate->getOutput(connection->output));
            updateQueue.push(connection->child);
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

    andGate.connect(&manager, &notGate, 0, 0);
    notGate.connect(&manager, &andGate, 0, 0);
    onGate.connect(&manager, &andGate, 0, 1);

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
