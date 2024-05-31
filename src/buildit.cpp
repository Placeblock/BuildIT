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
    for (size_t i = 0; i < gate->outputs.size(); i++) {
        if (gate->getOutput(i) != (oldOutput & (1 << i))) {
            for (const auto &child: gate->outputs[i]) {
                child->gate->setInput(child->index, gate->getOutput(i));
                updateQueue.push(child->gate);
            }
        }
    }
}

int main() {
    OnGate onGate;
    AndGate andGate;
    andGate.inputs = 2;
    andGate.recalcInputMask();
    NotGate notGate;
    notGate.inputs = 1;
    notGate.recalcInputMask();
    notGate.setOutput(0, true);

    pin_reference andPin{&andGate, 0};
    pin_reference andPin2{&andGate, 1};
    pin_reference notPin{&notGate, 0};
    andGate.connect(0, &notPin);
    notGate.connect(0, &andPin);
    onGate.connect(0, &andPin2);

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
        if (updates % 1000000 == 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
            std::cout << "Updates: " << (updates/elapsed) << " U/μs\n";
        }
    }

    return 0;
}
