#include <iostream>
#include <queue>
#include <map>
#include <chrono>
#include "tree.h"
#include <bitset>

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

    pin_reference andpin{&andGate, 0};
    pin_reference notpin{&notGate, 0};
    pin_reference onpin{&onGate, 0};
    andGate.outputs.emplace_back(std::vector<pin_reference*>{&notpin});
    notGate.inputs.emplace_back(&andpin);
    notGate.outputs.emplace_back(std::vector<pin_reference*>{&andpin});
    andGate.inputs.emplace_back(&notpin);
    andGate.inputs.emplace_back(&onpin);

    updateQueue.push(&andGate);

    int updates = 0;
    auto start = std::chrono::steady_clock::now();
    Gate *update_gate;
    while (!updateQueue.empty()) {
        update_gate = updateQueue.front();
        if (update_gate != nullptr) {
            update(update_gate);
        }
        updateQueue.pop();
        updates++;
        if (updates % 1000000 == 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count();
            std::cout << "Updates: " << (updates/elapsed) << " U/ms\n";
        }
    }

    return 0;
}
