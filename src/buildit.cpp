#include <iostream>
#include <queue>
#include <chrono>
#include "tree.h"

std::queue<Gate*> updateQueue;

void update(struct Gate *gate) {
    //printf("Updating gate\n");
    // Copying old output values for checking them later
    uint32_t oldOutput = gate->output;
    //std::cout << "Old output: " << std::bitset<32>(oldOutput) << "\n";

    // Updating all input values with parent output values
    gate->updateInputs();
    //std::cout << "Updated input: " << std::bitset<32>(gate->input) << "\n";
    // Update the gate
    gate->update();
    //std::cout << "New output: " << std::bitset<32>(gate->output) << "\n";
    // Update children of changed outputs
    for (size_t i = 0; i < gate->outputs.size(); i++) {
        if (gate->getOutput(i) != (oldOutput & (1 << i))) {
            for (const auto &child: gate->outputs[i]) {
                updateQueue.push(child->gate);
            }
        }
    }
}

int main() {
    OnGate onGate;
    AndGate andGate;
    NotGate notGate;
    notGate.setOutput(0, true);

    pin_reference andPin{&andGate, 0};
    pin_reference notPin{&notGate, 0};
    pin_reference onPin{&onGate, 0};
    andGate.outputs.emplace_back(std::vector<pin_reference*>{&notPin});
    notGate.inputs.emplace_back(&andPin);
    notGate.outputs.emplace_back(std::vector<pin_reference*>{&andPin});
    andGate.inputs.emplace_back(&notPin);
    andGate.inputs.emplace_back(&onPin);

    notGate.recalcOutputMask();
    andGate.recalcInputMask();
    andGate.recalcOutputMask();
    notGate.recalcInputMask();
    notGate.recalcOutputMask();

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
