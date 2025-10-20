//
// Created by felix on 19.04.25.
//

#include "simulation/graph.hpp"
#include "simulation/simulation.hpp"
#include <iostream>
#include <thread>

class not_node_t final : public sim::node_t {
public:
    sim::pin_sink_t in;
    sim::pin_t out;

    not_node_t() : in(0), out(0, new bool) {
    }

    ~not_node_t() override {
        delete static_cast<bool *>(out.value);
    }

    void update(const std::function<void(const sim::pin_t &pin)> &on_updated) override {
        if (!*static_cast<bool *>(this->in.pin_value) != *static_cast<bool *>(this->out.value)) {
            *static_cast<bool *>(this->out.value) = !*static_cast<bool *>(this->in.pin_value);
            on_updated(this->out);
        }
    }
};

long long updates = 0;

void measure() {
    long long last_value = updates;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const long long value = updates;
        std::cout << value - last_value << " updates/s\n";
        last_value = value;
    }
}

int main() {
    std::thread monitor_thread(measure);

    sim::simulation_t simulation;

    not_node_t and_node;
    sim::graph::connect(and_node.out, and_node, and_node.in);

    simulation.update(&and_node);
    while (!simulation.is_empty()) {
        simulation.poll_and_update();
        updates++;
    }
}