#ifndef TREE_H
#define TREE_H

#include <stdint.h>
#include <vector>

struct pin_reference {
    struct gate* gate;
    int index;
};

struct output_pin {
    bool state;
    std::vector<pin_reference*> children;
};

struct input_pin {
    bool state;
    pin_reference* parent;
};

struct gate {
    std::vector<input_pin*> inputs;
    std::vector<output_pin*> outputs;
    void (*update_ptr)(struct gate*);
};

#endif //TREE_H