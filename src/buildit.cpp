#include <iostream>
#include <queue>
#include <map>
#include <chrono>
#include "tree.h"
#include "gates.h"

std::queue<gate*> update_queue;

void update(struct gate *gate) {
    // Copying old output values for checking them later
    bool state[gate->outputs.size()];
    for (size_t i = 0; i < gate->outputs.size(); i++) {
        state[i] = gate->outputs[i]->state;
    }    
    // Updating all input values with parent output values
    for (auto input : gate->inputs) {
        if (input->parent != NULL) {
            struct gate *parent_gate = input->parent->gate;
            input->state = parent_gate->outputs[input->parent->index]->state;
        }
    }
    // Update the gate
    gate->update_ptr(gate);
    // Update chilren of changed outputs
    for (uint8_t i = 0; i < gate->outputs.size(); i++) {
        output_pin *output = gate->outputs[i];
        if (output->state != state[i]) {
            for (uint8_t j = 0; j < output->children.size(); j++) {
                update_queue.push(output->children[j]->gate);
            }
        }
    }
}

int main() {
    std::cout.imbue(std::locale(""));
    
    gate and_gate = create_gate(&and_gate_logic, 2, 1);
    gate not_gate = create_gate(&not_gate_logic, 1, 1);
    not_gate.outputs[0]->state = true;

    pin_reference andchild{&not_gate, 0};
    and_gate.outputs[0]->children.push_back(&andchild);
    and_gate.inputs[0]->parent = &andchild;
    pin_reference andparent{&and_gate, 0};
    not_gate.inputs[0]->parent = &andparent;
    not_gate.outputs[0]->children.push_back(&andparent);

    update_queue.push(&and_gate);

    int updates = 0;
    auto start = std::chrono::steady_clock::now();
    gate *update_gate;
    while (!update_queue.empty()) {
        update_gate = update_queue.front();
        if (update_gate != NULL) {
            update(update_gate);
        }
        update_queue.pop();
        updates++;
        if (updates % 1000000 == 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count();
            std::cout << "Updates: " << (updates/elapsed) << " U/ms\n";
        }
    }

    return 0;
}
