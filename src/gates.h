#ifndef GATES_H
#define GATES_H

#include <stdint.h>
#include <stddef.h>
#include "tree.h"

void and_gate_logic(struct gate *gate) {
    bool output = true;
    for (auto input : gate->inputs) {
        if (!input->state) {
            output = false;
            break;
        }
    }
    //printf("Switching and gate %s\n", output ? "on" : "off");
    for (size_t i = 0; i < gate->outputs.size(); i++) {
        gate->outputs[i]->state = output;
    }
}

void not_gate_logic(struct gate *gate) {
    for (size_t i = 0; i < gate->outputs.size(); i++) {
        bool output = !gate->inputs[i]->state;
        //printf("Switching not gate %s\n", output ? "on" : "off");
        gate->outputs[i]->state = output;
    }
}

void or_gate_logic(struct gate *gate) {
    bool output = false;
    for (auto input : gate->inputs) {
        if (input->state) {
            output = true;
            break;
        }
    }
    //printf("Switching or gate %s\n", output ? "on" : "off");
    for (size_t i = 0; i < gate->outputs.size(); i++) {
        gate->outputs[i]->state = output;
    }
}

gate create_gate(void (*update_ptr)(gate*), uint8_t input_size, uint8_t output_size) {
    std::vector<output_pin*> outputs(output_size);
    for (size_t o = 0; o < output_size; o++) {
        // FREE THIS SHIT LATER !!!!!!!!!!!!!!!!!!!
        output_pin* output = new output_pin{false};
        outputs[o] = output;
    }
    std::vector<input_pin*> inputs(input_size);
    for (size_t i = 0; i < input_size; i++) {
        // FREE THIS SHIT LATER !!!!!!!!!!!!!!!!!!!
        input_pin* input = new input_pin{true};
        inputs[i] = input;
    }
    gate new_gate{
        inputs,
        outputs,
        update_ptr
    };
    return new_gate;
}

#endif //GATES_H